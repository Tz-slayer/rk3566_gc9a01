#pragma once
#include "IDriverDisplay.h"
#include "ISPIInterface.h"
#include "IGPIOInterface.h"
#include "IDrawStrategy.h"
#include <cstdint>

// GC9A01显示屏操作接口（桥接模式的Abstraction）
class GC9A01Display : public IDriverDisplay {
private:
    ISPIInterface* spi_interface;
    IGPIOInterface* gpio_interface;

    uint16_t width;       // 屏幕宽度
    uint16_t height;      // 屏幕高度

    void writeReg(uint8_t reg);
    void writeData(uint8_t data);

    // 私有辅助函数：设置显示窗口（通用逻辑）
    void columnAddressSet(uint16_t y0, uint16_t y1);
    void rowAddressSet(uint16_t x0, uint16_t x1);
    void memeoryWrite();
    
public:
    // 构造函数：注入通信接口
    GC9A01Display(ISPIInterface* spi, IGPIOInterface* gpio, uint16_t w = 240, uint16_t h = 240) 
        : spi_interface(spi), gpio_interface(gpio), width(w), height(h) {}

    ~GC9A01Display() = default;
    void sendMemory(const std::vector<uint8_t>& data);
    uint16_t getWidth() const;
    uint16_t getHeight() const;
    // 初始化芯片（通用初始化流程）
    void init(uint8_t direction=2);
    void reSet();
    // 初始化显示屏
    void initDisplay(uint8_t direction=2);
    // 设置显示窗口
    void setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

};
