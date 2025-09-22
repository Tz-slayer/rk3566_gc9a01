#include "RGAImageProcessor.h"


int RGAImageProcessor::cvtColor(void *src_image, void *dst_image, int width, int height, int src_fmt, int dst_fmt) {
    // 方案1：使用 DMA 内存（推荐）
    int src_dma_fd;
    void* src_dma_va;
    size_t src_buf_size = width * height * get_bpp_from_format(src_fmt);
    // 获取源缓冲区
    DMABuffer* src_buf = dma_pool_->acquire(src_buf_size);
    dma_pool_->print_status();
    if (!src_buf) {
        printf("Failed to acquire source DMA buffer from pool\n");
        return -1;
    }
    // 复制数据到 DMA 内存
    memcpy(src_buf->va, src_image, src_buf_size);
    // 导入 DMA 缓冲区到 RGA
    rga_buffer_handle_t src_handle = importbuffer_fd(src_buf->fd, src_buf_size);
    if (src_handle == 0) {
        printf("import src dma_fd error!\n");
        return -1;
    }
    // 将句柄封装成完整的RGA缓冲区结构
    rga_buffer_t src = wrapbuffer_handle(src_handle, width, height, src_fmt);

    // 计算目标图像缓冲区大小
    size_t dst_buf_size = width * height * get_bpp_from_format(dst_fmt);
    // 获取目标缓冲区
    DMABuffer* dst_buf = dma_pool_->acquire(dst_buf_size);
    dma_pool_->print_status();
    if (!dst_buf) {
        printf("Failed to acquire destination DMA buffer from pool\n");
        releasebuffer_handle(src_handle);
        return -1;
    }
    // 将DMA描述符导入RGA
    rga_buffer_handle_t dst_handle = importbuffer_fd(dst_buf->fd, dst_buf_size);
    if (dst_handle == 0) {
        printf("import dst dma_fd error!\n");
        return -1;
    } 
    // 将句柄封装成完整的RGA缓冲区结构
    rga_buffer_t dst = wrapbuffer_handle(dst_handle, width, height, dst_fmt);

    // 检查缓冲区兼容性
    int ret = imcheck(src, dst, {}, {});
    if (IM_STATUS_NOERROR != ret) {
        printf("imcheck failed: %s\n", imStrError((IM_STATUS)ret));
        return -1;
    }
    
    // 转换颜色格式
    ret = imcvtcolor(src, dst, src_fmt, dst_fmt);
    if (ret != IM_STATUS_SUCCESS) {
        printf("imcvtcolor failed with code: %d, %s\n", ret, imStrError((IM_STATUS)ret));
        return -1;
    }
    return 0;
}


int RGAImageProcessor::resize(void *src_image, void *dst_image, int src_width, int src_height, int dst_width, int dst_height, int src_fmt, int dst_fmt) {
    // 方案1：使用 DMA 内存（推荐）
    int src_dma_fd;
    void* src_dma_va;
    size_t src_buf_size = src_width * src_height * get_bpp_from_format(src_fmt);
    // 获取源缓冲区
    DMABuffer* src_buf = dma_pool_->acquire(src_buf_size);
    dma_pool_->print_status();
    if (!src_buf) {
        printf("Failed to acquire source DMA buffer from pool\n");
        return -1;
    }
    // 复制数据到 DMA 内存
    memcpy(src_buf->va, src_image, src_buf_size);
    // 导入 DMA 缓冲区到 RGA
    rga_buffer_handle_t src_handle = importbuffer_fd(src_buf->fd, src_buf_size);
    if (src_handle == 0) {
        printf("import src dma_fd error!\n");
        return -1;
    }
    // 将句柄封装成完整的RGA缓冲区结构
    rga_buffer_t src = wrapbuffer_handle(src_handle, src_width, src_height, src_fmt);

    // 计算目标图像缓冲区大小
    size_t dst_buf_size = dst_width * dst_height * get_bpp_from_format(dst_fmt);
    // 获取目标缓冲区
    DMABuffer* dst_buf = dma_pool_->acquire(dst_buf_size);
    dma_pool_->print_status();
    if (!dst_buf) {
        printf("Failed to acquire destination DMA buffer from pool\n");
        releasebuffer_handle(src_handle);
        return -1;
    }
    // 将DMA描述符导入RGA
    rga_buffer_handle_t dst_handle = importbuffer_fd(dst_buf->fd, dst_buf_size);
    if (dst_handle == 0) {
        printf("import dst dma_fd error!\n");
        return -1;
    } 
    // 将句柄封装成完整的RGA缓冲区结构
    rga_buffer_t dst = wrapbuffer_handle(dst_handle, dst_width, dst_height, dst_fmt);

    // 检查缓冲区兼容性
    int ret = imcheck(src, dst, {}, {});
    if (IM_STATUS_NOERROR != ret) {
        printf("imcheck failed: %s\n", imStrError((IM_STATUS)ret));
        return -1;
    }
    
    // 转换颜色格式
    ret = imresize(src, dst);
    if (ret != IM_STATUS_SUCCESS) {
        printf("imcvtcolor failed with code: %d, %s\n", ret, imStrError((IM_STATUS)ret));
        return -1;
    }
    return 0;
}