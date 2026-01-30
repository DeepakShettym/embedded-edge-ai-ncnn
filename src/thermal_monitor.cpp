#include "thermal_monitor.h"
#include <fstream>

static float read_temp(const char* path) {
    std::ifstream f(path);
    int temp = 0;
    if (f.is_open()) {
        f >> temp;
        f.close();
    }
    return temp / 1000.0f;
}

float get_cpu_temp() {
    float t_l = read_temp("/sys/class/thermal/thermal_zone0/temp");
    float t_b = read_temp("/sys/class/thermal/thermal_zone1/temp");
    return (t_l > t_b) ? t_l : t_b;
}
