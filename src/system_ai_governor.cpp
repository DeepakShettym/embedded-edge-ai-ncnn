#include <ncnn/net.h>

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <atomic>
#include <thread>
#include <mutex>
#include <fstream>
#include <string>
#include <cstring>

#include <sys/socket.h>
#include <sys/un.h>

// =======================
// Embedded sensing helpers
// =======================

float read_cpu_temp()
{
    std::ifstream f("/sys/class/thermal/thermal_zone0/temp");
    int temp = 0;
    f >> temp;
    return temp / 1000.0f;
}

float read_cpu_load()
{
    static long prev_idle = 0, prev_total = 0;

    std::ifstream f("/proc/stat");
    std::string cpu;
    long user, nice, system, idle, iowait, irq, softirq;

    f >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq;

    long idle_now = idle + iowait;
    long total_now = user + nice + system + idle + iowait + irq + softirq;

    long diff_idle = idle_now - prev_idle;
    long diff_total = total_now - prev_total;

    prev_idle = idle_now;
    prev_total = total_now;

    if (diff_total == 0) return 0.0f;
    return 100.0f * (1.0f - (float)diff_idle / diff_total);
}

// =======================
// AI Governor State
// =======================

enum class AiMode {
    AUTO = 0,     // original behavior
    FULL = 1,     // force full AI
    LIMITED = 2,  // throttled AI
    OFF = 3       // AI disabled
};

std::atomic<AiMode> current_mode(AiMode::AUTO);
std::atomic<bool> trigger_inference(false);
std::atomic<bool> running(true);

std::mutex status_mutex;
float current_temp = 0.0f;
float current_cpu = 0.0f;

// =======================
// IPC: Unix domain socket
// =======================

void socket_listener()
{
    const char* socket_path = "/data/local/tmp/system_ai.sock";
    unlink(socket_path);

    int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return;
    }

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(server_fd);
        return;
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen");
        close(server_fd);
        return;
    }

    printf("[GOVERNOR] IPC socket ready\n");

    while (running) {
        int client = accept(server_fd, nullptr, nullptr);
        if (client < 0) continue;

        char buf[128] = {0};
        read(client, buf, sizeof(buf) - 1);
        std::string cmd(buf);
        std::string reply;

        if (cmd.find("STATUS") == 0) {
            std::lock_guard<std::mutex> lock(status_mutex);
            reply =
                "TEMP=" + std::to_string(current_temp) +
                " CPU=" + std::to_string(current_cpu) +
                " MODE=" + std::to_string((int)current_mode.load()) + "\n";
        }
        else if (cmd.find("SET_MODE AUTO") == 0) {
            current_mode = AiMode::AUTO;
            reply = "OK MODE AUTO\n";
        }
        else if (cmd.find("SET_MODE FULL") == 0) {
            current_mode = AiMode::FULL;
            reply = "OK MODE FULL\n";
        }
        else if (cmd.find("SET_MODE LIMITED") == 0) {
            current_mode = AiMode::LIMITED;
            reply = "OK MODE LIMITED\n";
        }
        else if (cmd.find("SET_MODE OFF") == 0) {
            current_mode = AiMode::OFF;
            reply = "OK MODE OFF\n";
        }
        else if (cmd.find("RUN_INFER") == 0) {
            trigger_inference = true;
            reply = "OK INFERENCE TRIGGERED\n";
        }
        else {
            reply = "ERROR UNKNOWN COMMAND\n";
        }

        write(client, reply.c_str(), reply.size());
        close(client);
    }

    close(server_fd);
}

// =======================
// Signal handling
// =======================

void handle_signal(int)
{
    running = false;
}

// =======================
// MAIN
// =======================

int main()
{
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    printf("System AI Governor starting...\n");

    // Start IPC listener
    std::thread ipc(socket_listener);
    ipc.detach();

    // NCNN setup
    ncnn::Net net;
    net.opt.use_vulkan_compute = false;

    while (running)
    {
        float temp = read_cpu_temp();
        float cpu  = read_cpu_load();

        {
            std::lock_guard<std::mutex> lock(status_mutex);
            current_temp = temp;
            current_cpu  = cpu;
        }

        printf("Temp: %.1f°C | CPU: %.1f%% | ", temp, cpu);

        AiMode mode = current_mode.load();

        if (mode == AiMode::OFF) {
            printf("MODE: OFF (AI disabled)\n");
            sleep(3);
            continue;
        }

        if (mode == AiMode::FULL) {
            net.opt.num_threads = 4;
            printf("MODE: FULL (forced)\n");
            printf("AI inference executed\n");
            sleep(1);
            continue;
        }

        if (mode == AiMode::LIMITED) {
            net.opt.num_threads = 2;
            printf("MODE: LIMITED (forced)\n");
            printf("AI inference executed (throttled)\n");
            sleep(2);
            continue;
        }

        // AUTO mode (your original logic)
        if (temp < 60 && cpu < 60) {
            net.opt.num_threads = 4;
            printf("MODE: AUTO → FULL\n");
            printf("AI inference executed\n");
            sleep(1);
        }
        else if (temp < 70) {
            net.opt.num_threads = 2;
            printf("MODE: AUTO → LIMITED\n");
            printf("AI inference executed (throttled)\n");
            sleep(2);
        }
        else {
            printf("MODE: AUTO → SURVIVAL (AI paused)\n");
            sleep(3);
        }

        // One-shot inference trigger
        if (trigger_inference.exchange(false)) {
            printf("Manual inference trigger\n");
        }
    }

    printf("System AI Governor shutting down\n");
    return 0;
}

