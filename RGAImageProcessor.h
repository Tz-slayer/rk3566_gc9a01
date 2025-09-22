#pragma once
#include "dma_alloc.h"
#include "im2d.h"
#include "RgaUtils.h"
#include "DMAPool.h"
#include <cstring>

class DMAHandleGuard {
private:
    DMAPool* dma_pool_;
    DMABuffer* buf_;
    rga_buffer_handle_t handle_;

public:
    DMAHandleGuard(DMAPool* pool, size_t size)
        : dma_pool_(pool), buf_(nullptr), handle_(0) {
        buf_ = dma_pool_->acquire(size);
        if (buf_) {
            handle_ = importbuffer_fd(buf_->fd, size);
            if (handle_ == 0) {
                dma_pool_->release(buf_);
                buf_ = nullptr;
            }
        }
    }

    ~DMAHandleGuard() {
        if (handle_) releasebuffer_handle(handle_);
        // 由于函数中不希望结束调资源的生命周期，释放资源，所以暂时注释掉
        // if (buf_) dma_pool_->release(buf_);
    }

    DMABuffer* buffer() const { return buf_; }
    rga_buffer_handle_t handle() const { return handle_; }
    bool valid() const { return buf_ && handle_; }
};

class RGAImageProcessor {
public:
    RGAImageProcessor(DMAPool &dma_pool) : dma_pool_(&dma_pool) {}
    ~RGAImageProcessor() = default;
    // 颜色转换
    DMABuffer* cvtColor(void *src_image, int width, int height, int src_fmt, int dst_fmt);
    // 图像变换
    DMABuffer* resize(void *src_image, int src_width, int src_height, int dst_width, int dst_height, int src_fmt, int dst_fmt);

private:
    DMAPool *dma_pool_; // DMA 缓冲区池
};
