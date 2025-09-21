#pragma once
#include <cstdint>
#include <vector>

// 绘图策略接口（策略模式的Strategy）
class IDrawStrategy {
public:
    IDrawStrategy() {}
    virtual ~IDrawStrategy() = default;
    // 使用指定颜色清屏
    virtual void clear(uint16_t color) = 0;
    // 显示图片（RGB565格式）
    virtual void displayImage(const uint16_t* image, std::vector<uint16_t> region) = 0;
};