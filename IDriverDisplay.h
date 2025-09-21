#pragma once
#include <cstdint>
#include <vector>

// 屏幕芯片驱动接口（抽象出通用驱动功能）
class IDriverDisplay {
public:
    IDriverDisplay() {}
    virtual ~IDriverDisplay() = default;

    // 初始化显示屏（包括芯片初始化和方向设置）
    virtual void init(uint8_t direction = 2) = 0;

    // 发送内存数据到显示屏
    virtual void sendMemory(const std::vector<uint8_t>& data) = 0;

    // 设置显示窗口（定义绘图区域）
    virtual void setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) = 0;

    // 获取显示屏宽度
    virtual uint16_t getWidth() const = 0;

    // 获取显示屏高度
    virtual uint16_t getHeight() const = 0;
};