#pragma once 

#include<string>


class LedController
{

public:
    explicit  LedController(const std::string& brightness_path);
    void on();
    void off();


private:

    std::string path;
    void write(const char *value);

};




