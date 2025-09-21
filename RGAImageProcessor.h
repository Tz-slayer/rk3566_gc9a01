#pragma once
#include "IImageProcessor.h"

// RGA图像处理器
class RGAImageProcessor : public IImageProcessor {
public:
    RGAImageProcessor() {}
    virtual ~RGAImageProcessor() = default;

    // 实现IImageProcessor接口
    std::size_t getWidth() const override {
        // 返回图像宽度
    }

    std::size_t getHeight() const override {
        // 返回图像高度
    }

    const uint8_t* getData() const override {
        // 返回图像数据
    }
};