#include "../GC9A01Display.h"
#include "../BasicDrawStrategy.h"
#include "../OrangePi3BGPIO.h"
#include "../OrangePi3BSPI.h"
#include <iostream>
#include <vector>
#include <unistd.h>
#include <chrono>  // 添加计时库

int main() {
    uint8_t LED = 10;
    uint8_t DC = 8;
    uint8_t RST = 5;
    uint8_t CS = 7;
    uint8_t SDA = 11;
    uint8_t SCL = 12;

    // 创建 GPIO 对象（参数根据硬件调整）
    OrangePi3BGPIO gpio(LED, DC, RST, CS, SDA, SCL);
    if (!gpio.init()) {  // 假设有 init() 方法
        std::cout << "GPIO init failed" << std::endl;
        return 1;
    }

    // 创建 SPI 对象
    OrangePi3BSPI spi(3, 0, 1000 * 1000 * 40);
    if (!spi.init()) {
        std::cout << "SPI init failed" << std::endl;
        return 1;
    }

    // 创建显示对象（假设 240x240 屏幕）
    GC9A01Display display(&spi, &gpio, 240, 240);

    // 创建绘制策略
    BasicDrawStrategy drawStrategy(&display);

    // 测试初始化
    std::cout << "Testing init..." << std::endl;
    display.initDisplay(2);  // 默认方向

    // 测试设置窗口
    std::cout << "Testing setWindow..." << std::endl;
    display.setWindow(0, 0, 239, 239);

    // 测试绘制策略（清屏），并测量 sendMemory 时间
    std::cout << "Testing draw strategy clear..." << std::endl;
    
    // 预计算数据（模拟 chunk）
    std::vector<uint8_t> chunk(4096, 0xFF);  // 示例 chunk，大小 4096 字节
    
    // 测量 sendMemory 时间
    auto start = std::chrono::high_resolution_clock::now();
    display.sendMemory(chunk);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "sendMemory time: " << duration.count() << " ms" << std::endl;

    usleep(1000000); // 等待1秒

    // 完整清屏测试（可选）
    drawStrategy.clear(0xFFFF);  // 清屏为白色

    std::cout << "Test completed." << std::endl;
    return 0;
}