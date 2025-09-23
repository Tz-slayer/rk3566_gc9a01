#include "RGAImageProcessor.h"

DMABuffer* RGAImageProcessor::cvtColor(void *src_image, int width, int height, int src_fmt, int dst_fmt) {
    size_t src_buf_size = width * height * get_bpp_from_format(src_fmt);
    DMAHandleGuard src_guard(dma_pool_, src_buf_size);
    if (!src_guard.valid()) {
        printf("Failed to acquire source DMA buffer\n");
        return nullptr;
    }
    memcpy(src_guard.buffer()->va, src_image, src_buf_size);
    rga_buffer_t src = wrapbuffer_handle(src_guard.handle(), width, height, src_fmt);

    size_t dst_buf_size = width * height * get_bpp_from_format(dst_fmt);
    // void *dst_image = new char[dst_buf_size]; // 分配目标图像内存
    DMAHandleGuard dst_guard(dma_pool_, dst_buf_size);
    if (!dst_guard.valid()) {
        printf("Failed to acquire destination DMA buffer\n");
        return nullptr;
    }
    rga_buffer_t dst = wrapbuffer_handle(dst_guard.handle(), width, height, dst_fmt);

    // int ret = imcheck(src, dst, {}, {});
    // if (IM_STATUS_NOERROR != ret) {
    //     printf("imcheck failed: %s\n", imStrError((IM_STATUS)ret));
    //     return nullptr;
    // }

    int ret = imcvtcolor(src, dst, src_fmt, dst_fmt);
    if (ret != IM_STATUS_SUCCESS) {
        printf("imcvtcolor failed: %s\n", imStrError((IM_STATUS)ret));
        return nullptr;
    }
    // 释放掉函数内部使用的临时dmabuffer
    dma_pool_->release(src_guard.buffer());

    // if (dst_guard.buffer()->va) {
    //     // memcpy(dst_image, dst_guard.buffer()->va, dst_buf_size);
    // }
    return dst_guard.buffer();  // 资源自动释放，无需手动
}

DMABuffer* RGAImageProcessor::resize(void *src_image, int src_width, int src_height, int dst_width, int dst_height, int src_fmt, int dst_fmt) {
    size_t src_buf_size = src_width * src_height * get_bpp_from_format(src_fmt);
    DMAHandleGuard src_guard(dma_pool_, src_buf_size);
    if (!src_guard.valid()) {
        printf("Failed to acquire source DMA buffer\n");
        return nullptr;
    }
    memcpy(src_guard.buffer()->va, src_image, src_buf_size);
    rga_buffer_t src = wrapbuffer_handle(src_guard.handle(), src_width, src_height, src_fmt);

    size_t dst_buf_size = dst_width * dst_height * get_bpp_from_format(dst_fmt);
    // void *dst_image = new char[dst_buf_size]; // 分配目标图像内存
    DMAHandleGuard dst_guard(dma_pool_, dst_buf_size);
    if (!dst_guard.valid()) {
        printf("Failed to acquire destination DMA buffer\n");
        return nullptr;
    }
    rga_buffer_t dst = wrapbuffer_handle(dst_guard.handle(), dst_width, dst_height, dst_fmt);

    // int ret = imcheck(src, dst, {}, {});
    // if (IM_STATUS_NOERROR != ret) {
    //     printf("imcheck failed: %s\n", imStrError((IM_STATUS)ret));
    //     return nullptr;
    // }

    int ret = imresize(src, dst);
    if (ret != IM_STATUS_SUCCESS) {
        printf("imresize failed: %s\n", imStrError((IM_STATUS)ret));
        return nullptr;
    }

    // 释放掉函数内部使用的临时dmabuffer
    dma_pool_->release(src_guard.buffer());

    // if (dst_guard.buffer()->va) {
    //     memcpy(dst_image, dst_guard.buffer()->va, dst_buf_size);
    // }
    return dst_guard.buffer();  // 资源自动释放，无需手动
}