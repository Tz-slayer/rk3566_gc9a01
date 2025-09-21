#include "../OrangePi3BGPIO.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <wiringPi.h>
#include <unistd.h>

int main() {
    // 假设引脚编号如下，根据实际硬件修改
    uint8_t LED = 10;
    uint8_t DC = 8;
    uint8_t RST = 5;
    uint8_t CS = 7;
    uint8_t SDA = 11;
    uint8_t SCL = 12;


    // if (geteuid () != 0)
    // {
    //     fprintf (stderr, "Must be root to run. Program should be suid root. This is an error.\n") ;
    //     exit (EXIT_FAILURE) ;
    // }

    // if (wiringPiSetup () == -1)
    // {
    //     fprintf (stdout, "WiringPi setup failed. Are you running as root??\n") ;
    //     exit (EXIT_FAILURE) ;
    // }

    OrangePi3BGPIO gpio(LED, DC, RST, CS, SDA, SCL);

    if (!gpio.init()) {
        std::cerr << "GPIO init failed!" << std::endl;
        return 1;
    }


    // 测试LED引脚高低电平
    std::cout << "Set LED HIGH" << std::endl;
    gpio.setPin(LED, true);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 读取LED引脚电平
    bool state = gpio.getPin(LED);
    std::cout << "LED pin state: " << state << std::endl;

    std::cout << "Set LED LOW" << std::endl;
    gpio.setPin(LED, false);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 读取LED引脚电平
    state = gpio.getPin(LED);
    std::cout << "LED pin state: " << state << std::endl;

    // 设置引脚模式
    gpio.setPinMode(LED, 0); // 1: OUTPUT

    gpio.close();
    std::cout << "GPIO test finished." << std::endl;
    return 0;
}