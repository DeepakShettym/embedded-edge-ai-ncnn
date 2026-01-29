#include <ncnn/net.h>
#include <unistd.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <ctime>

float read_cpu_temp()
{
    std::ifstream f("/sys/class/thermal/thermal_zone0/temp");
    int temp;
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

    if (diff_total == 0) return 0;
    return 100.0f * (1.0f - (float)diff_idle / diff_total);
}

int main()
{
    printf("System AI Governor starting...\n");

    ncnn::Net net;
    net.opt.use_vulkan_compute = false;

    while (1)
    {
        float temp = read_cpu_temp();
        float load = read_cpu_load();

        printf("Temp: %.1f°C | CPU Load: %.1f%% | ", temp, load);

        if (temp < 60 && load < 60)
        {
            net.opt.num_threads = 4;
            printf("MODE: FULL AI\n");
            printf("AI inference executed\n");
            sleep(1);
        }
        else if (temp < 70)
        {
            net.opt.num_threads = 2;
            printf("MODE: LIMITED AI\n");
            printf("AI inference executed (throttled)\n");
            sleep(2);
        }
        else
        {
            printf("MODE: SURVIVAL (AI paused)\n");
            sleep(3);
        }
    }
}
