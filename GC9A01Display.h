#pragma once
#include "ISPIInterface.h"
#include "IGPIOInterface.h"
#include "IDrawStrategy.h"
#include <cstdint>

// GC9A01显示屏操作接口（桥接模式的Abstraction）
class GC9A01Display {
private:
    ISPIInterface* comm_interface;
    IGPIOInterface* gpio_interface;
    IDrawStrategy* draw_strategy;
    uint16_t width;       // 屏幕宽度
    uint16_t height;      // 屏幕高度

    // 私有辅助函数：设置显示窗口（通用逻辑）
    void columnAddressSet(uint16_t y0, uint16_t y1);
    void rowAddressSet(uint16_t x0, uint16_t x1);
    void memeoryWrite();
    
public:
    // 构造函数：注入通信接口
    GC9A01Display(ISPIInterface* comm, IGPIOInterface* gpio, IDrawStrategy* draw, uint16_t w = 240, uint16_t h = 240) 
        : comm_interface(comm), gpio_interface(gpio), draw_strategy(draw), width(w), height(h) {}

    ~GC9A01Display() = default;

    // 初始化显示屏（通用初始化流程）
    void init(uint8_t direction=2);
    // 设置显示窗口
    void setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    // 清屏
    void clear(uint16_t color = 0x0000);
    // 显示图片（RGB565格式）
    void displayImage(const uint16_t* image, std::vector<uint16_t> region);
};
