#pragma once
#include <cstdint>
#include <cstddef>
#include "IDrawStrategy.h"
#include "IDriverDisplay.h"

class BasicDrawStrategy : public IDrawStrategy {
private:
    IDriverDisplay* display; // 指向IDriverDisplay实例的指针

public:
    BasicDrawStrategy(IDriverDisplay* disp) : display(disp) {}
    ~BasicDrawStrategy() override;

    // 数据分块
    void sendInChunks(const std::vector<uint8_t>& data, size_t chunkSize);
    // 使用指定颜色清屏
    void clear(uint16_t color) override;
    // 显示图片（RGB565格式）
    void displayImage(const uint16_t* image, std::vector<uint16_t> region) override;
    // 显示图片序列
    void displayImageSequence(const std::vector<uint16_t*>& image, std::vector<std::vector<uint16_t>> regions, size_t delayMs = 100) override;
};