#pragma once
#include <cstdint>
#include <cstddef>
#include "IDrawStrategy.h"
#include "GC9A01Display.h"

class BasicDrawStrategy : public IDrawStrategy {
private:
    GC9A01Display* display; // 指向GC9A01Display实例的指针

public:
    BasicDrawStrategy(GC9A01Display* disp) : display(disp) {}
    ~BasicDrawStrategy() override = default;

    // 数据分块
    void sendInChunks(const std::vector<uint8_t>& data, size_t chunkSize);
    // 使用指定颜色清屏
    void clear(uint16_t color) override;
    // 显示图片（RGB565格式）
    void displayImage(const uint16_t* image, std::vector<uint16_t> region) override;
};