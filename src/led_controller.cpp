#include "led_controller.h"
#include <fstream>

LedController::LedController(const std::string& brightness_path)
    : path(brightness_path) {}

void LedController::write(const char* value) {
    std::ofstream fs(path);
    if (fs.is_open()) {
        fs << value;
        fs.close();
    }
}

void LedController::on() {
    write("0");   // ACTIVE LOW
}

void LedController::off() {
    write("1");
}
