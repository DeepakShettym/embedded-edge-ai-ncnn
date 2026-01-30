#include <unistd.h>
#include "led_controller.h"
#include "thermal_monitor.h"

int main() {
    LedController led("/sys/class/leds/green/brightness");

    while (true) {
        float temp = get_cpu_temp();

        if (temp > 65.0f) {
            led.on();
        } else {
            led.off();
        }

        usleep(500 * 1000);
    }
}

