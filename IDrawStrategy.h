#pragma once
#include <cstdint>
#include <vector>

// 绘图策略接口（策略模式的Strategy）
class IDrawStrategy {
public:
    IDrawStrategy() {}
    virtual ~IDrawStrategy() = default;
    // 绘制像素点
    virtual void drawPixel(int x, int y, uint16_t color) = 0;
    // 绘制线条
    virtual void drawLine(int x0, int y0, int x1, int y1, uint16_t color) = 0;
    // 绘制矩形
    virtual void drawRect(int x, int y, int w, int h, uint16_t color) = 0;
    // 填充矩形
    virtual void fillRect(int x, int y, int w, int h, uint16_t color) = 0;
    // 绘制圆形
    virtual void drawCircle(int x0, int y0, int r, uint16_t color) = 0;
    // 填充圆形
    virtual void fillCircle(int x0, int y0, int r, uint16_t color) = 0;
};