#pragma once
#include <cstdint>
#include <cstddef>

// 图像处理接口抽象类
class IImageProcessor {
public:
    IImageProcessor() {}
    virtual ~IImageProcessor() = default;
    // 获取图像宽度
    virtual std::size_t getWidth() const = 0;
    // 获取图像高度
    virtual std::size_t getHeight() const = 0;
    // 获取图像数据
    virtual const uint8_t* getData() const = 0;
};