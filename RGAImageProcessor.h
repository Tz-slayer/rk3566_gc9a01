#pragma once
#include "dma_alloc.h"
#include "im2d.h"
#include "RgaUtils.h"
#include "DMAPool.h"
#include <cstring>

class RGAImageProcessor {
public:
    RGAImageProcessor(DMAPool &dma_pool) : dma_pool_(&dma_pool) {}
    ~RGAImageProcessor() = default;
    // 颜色转换
    int cvtColor(void *src_image, void *dst_image, int width, int height, int src_fmt, int dst_fmt);
    // 图像变换
    int resize(void *src_image, void *dst_image, int src_width, int src_height, int dst_width, int dst_height, int src_fmt, int dst_fmt);

private:
    DMAPool *dma_pool_; // DMA 缓冲区池
};
