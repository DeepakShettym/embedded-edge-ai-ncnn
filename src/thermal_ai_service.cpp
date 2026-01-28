#include <ncnn/net.h>
#include <ncnn/mat.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#define INFERENCE_DEADLINE_MS 50

// Read CPU temperature from sysfs
int read_cpu_temp() {
    int fd = open("/sys/class/thermal/thermal_zone0/temp", O_RDONLY);
    if (fd < 0) return -1;

    char buf[32];
    int len = read(fd, buf, sizeof(buf) - 1);
    close(fd);

    if (len <= 0) return -1;

    buf[len] = '\0';
    return atoi(buf);  // millidegree Celsius
}

int main() {
    printf("Thermal-Aware AI Service starting...\n");

    // Load AI model ONCE
    ncnn::Net net;
    net.opt.use_vulkan_compute = false;
    net.opt.num_threads = 4;

    if (net.load_param("/data/local/tmp/squeezenet_v1.1.param") != 0 ||
        net.load_model("/data/local/tmp/squeezenet_v1.1.bin") != 0) {
        printf(" Failed to load AI model\n");
        return -1;
    }

    printf("AI model loaded\n");

    // Dummy input (image replaced later if needed)
    ncnn::Mat input(227, 227, 3);
    input.fill(0.5f);

    while (1) {
        int temp = read_cpu_temp();

        if (temp < 0) {
            printf("Failed to read temperature\n");
            sleep(2);
            continue;
        }

        float temp_c = temp / 1000.0f;
        printf("CPU Temp: %.1f°C\n", temp_c);

        //Thermal policy
        if (temp_c > 65.0f) {
            printf("TOO HOT → Pausing AI\n");
            sleep(5);
            continue;
        }

        if (temp_c > 55.0f) {
            printf(" Warm → Reducing AI speed\n");
            net.opt.num_threads = 1;
            sleep(2);
        } else {
            printf(" Normal → Full AI speed\n");
            net.opt.num_threads = 4;
        }

        // Run inference
        // -----------------------------
	// Real-time inference timing
	// -----------------------------

	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);


	ncnn::Extractor ex = net.create_extractor();
	ex.input("data", input);

	ncnn::Mat output;
	ex.extract("prob", output);

	clock_gettime(CLOCK_MONOTONIC, &end);

	// Calculate inference time in ms
	long inference_ms =
   	 (end.tv_sec - start.tv_sec) * 1000 +
    	(end.tv_nsec - start.tv_nsec) / 1000000;

// Deadline check
	if (inference_ms > INFERENCE_DEADLINE_MS) {
    	printf("Deadline MISSED: %ld ms (limit %d ms) → Result DROPPED\n",
           inference_ms, INFERENCE_DEADLINE_MS);
	} else {
    	printf(" Inference OK: %ld ms (within deadline)\n", inference_ms);
	}

        sleep(2);
    }

    return 0;
}
