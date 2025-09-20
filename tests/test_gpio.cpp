#include "OrangePi3BGPIO.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    // 假设引脚编号如下，根据实际硬件修改
    uint8_t LED = 129;
    uint8_t DC = 130;
    uint8_t RST = 118;
    uint8_t CS = 128;
    uint8_t SDA = 138;
    uint8_t SCL = 136;

    OrangePi3BGPIO gpio(LED, DC, RST, CS, SDA, SCL);

    if (!gpio.init()) {
        std::cerr << "GPIO init failed!" << std::endl;
        return 1;
    }

    // 测试LED引脚高低电平
    std::cout << "Set LED HIGH" << std::endl;
    gpio.setPin(LED, true);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Set LED LOW" << std::endl;
    gpio.setPin(LED, false);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 读取LED引脚电平
    bool state = gpio.getPin(LED);
    std::cout << "LED pin state: " << state << std::endl;

    // 设置引脚模式
    gpio.setPinMode(LED, 1); // 1: OUTPUT

    gpio.close();
    std::cout << "GPIO test finished." << std::endl;
    return 0;
}