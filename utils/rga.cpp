#include "rga.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "dma_alloc.h"
#include <opencv2/opencv.hpp>

/**
 * @Description: 将图像导入 RGA 内部统一管理内存，而不是用户自己管理
 * @param {Mat} &image: 
 * @param {Mat} &resized_image: 
 * @param {Size} &target_size: 
 * @return {*}
 */
int RGA_handle_convert(void *src_image, void *dst_image, size_t src_width, size_t src_height, int src_format, int dst_format) {
    void *src_buf, *dst_buf;
    rga_buffer_t src_img, dst_img;
    rga_buffer_handle_t src_handle, dst_handle;
    size_t dst_width = src_width;
    size_t dst_height = src_height;
    int src_buf_size, dst_buf_size;
    int src_dma_fd, dst_dma_fd;
    // int src_format = RK_FORMAT_RGB_888;
    // int dst_format = RK_FORMAT_RGB_888;

    memset(&src_img, 0, sizeof(src_img));
    memset(&dst_img, 0, sizeof(dst_img));
    src_buf_size = src_width * src_height * get_bpp_from_format(src_format);
    dst_buf_size = dst_width * dst_height * get_bpp_from_format(dst_format);

    memcpy(src_buf, src_image, src_buf_size);

 
    /* 将缓冲区对应的物理地址信息映射到RGA驱动内部，并获取缓冲区相应的地址信息 */
    // 直接操作Mat.data内存，减少数据复制开销
    src_handle = importbuffer_virtualaddr(image, src_buf_size);
    dst_handle = importbuffer_virtualaddr(resized_image, dst_buf_size);
    if (src_handle == 0 || dst_handle == 0) {
        printf("importbuffer failed!\n");
        return -1;
    }
 
    /* 封装为RGA图像结构 */
    src_img = wrapbuffer_handle(src_handle, src_width, src_height, src_format);
    dst_img = wrapbuffer_handle(dst_handle, dst_width, dst_height, dst_format);
 
    IM_STATUS STATUS;
    /*STATUS = imcheck(src_img, dst_img, {}, {});
    if (IM_STATUS_NOERROR != STATUS) {
        printf("%d, check error! %s", __LINE__, imStrError(STATUS));
        return -1;
    }*/
 
    /* 执行缩放操作 */ 
    // STATUS = imresize(src_img, dst_img);
    STATUS = imcvtcolor(src_img, dst_img, src_format, dst_format);
 
    /* 释放内存（正确和错误均执行） */ 
    if (src_handle)
        releasebuffer_handle(src_handle);
    if (dst_handle)
        releasebuffer_handle(dst_handle);
 
    if (IM_STATUS_SUCCESS != STATUS) {
        fprintf(stderr, "rga resize error! %s", imStrError(STATUS));
        return -1;
    }
    return 0;
}

int read_image_from_file(void *buf, const char *path, int sw, int sh, int fmt, int index) {
    int size;

    FILE *file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr, "Could not open %s\n", path);
        return -1;
    }
    size = sw * sh * get_bpp_from_format(fmt);
    fread(buf, size, 1, file);
    fclose(file);

    return 0;
}

// 方案1：调用者分配内存
int convert_color_from_file_v1(void *src_buf, void *dst_buf, int src_width, int src_height, 
                               int src_format, int dst_format) {
    int ret = 0;
    int src_buf_size, dst_buf_size;

    rga_buffer_t src_img, dst_img;
    rga_buffer_handle_t src_handle = 0, dst_handle = 0;

    memset(&src_img, 0, sizeof(src_img));
    memset(&dst_img, 0, sizeof(dst_img));

    int dst_width = src_width;
    int dst_height = src_height;

    src_buf_size = src_width * src_height * get_bpp_from_format(src_format);
    dst_buf_size = dst_width * dst_height * get_bpp_from_format(dst_format);

    printf("Source buffer size: %d bytes\n", src_buf_size);
    printf("Destination buffer size: %d bytes\n", dst_buf_size);

    memset(dst_buf, 0x80, dst_buf_size);

    // 导入缓冲区
    src_handle = importbuffer_virtualaddr(src_buf, src_buf_size);
    dst_handle = importbuffer_virtualaddr(dst_buf, dst_buf_size);
    if (src_handle == 0 || dst_handle == 0) {
        printf("importbuffer failed!\n");
        ret = -1;
        goto release_buffer;
    }

    src_img = wrapbuffer_handle(src_handle, src_width, src_height, src_format);
    dst_img = wrapbuffer_handle(dst_handle, dst_width, dst_height, dst_format);

    ret = imcheck(src_img, dst_img, {}, {});
    if (IM_STATUS_NOERROR != ret) {
        printf("%d, check error! %s\n", __LINE__, imStrError((IM_STATUS)ret));
        ret = -1;
        goto release_buffer;
    }

    ret = imcvtcolor(src_img, dst_img, src_format, dst_format);
    if (ret == IM_STATUS_SUCCESS) {
        printf("imcvtcolor running success!\n");
        ret = 0;
    } else {
        printf("imcvtcolor running failed, %s\n", imStrError((IM_STATUS)ret));
        ret = -1;
    }

release_buffer:
    if (src_handle) releasebuffer_handle(src_handle);
    if (dst_handle) releasebuffer_handle(dst_handle);

    return ret;
}

// 方案2：函数分配内存，返回指针
int convert_color_from_file_v2(const char *path, void **dst_buf, int *dst_size, int src_width, int src_height,
                               int src_format, int dst_format) {
    int ret = 0;
    char *src_buf = nullptr;
    char *allocated_dst_buf = nullptr;
    int src_buf_size, dst_buf_size;

    rga_buffer_t src_img, dst_img;
    rga_buffer_handle_t src_handle = 0, dst_handle = 0;

    memset(&src_img, 0, sizeof(src_img));
    memset(&dst_img, 0, sizeof(dst_img));

    int dst_width = src_width;
    int dst_height = src_height;

    src_buf_size = src_width * src_height * get_bpp_from_format(src_format);
    dst_buf_size = dst_width * dst_height * get_bpp_from_format(dst_format);

    // 分配缓冲区
    src_buf = (char *)malloc(src_buf_size);
    allocated_dst_buf = (char *)malloc(dst_buf_size);
    if (!src_buf || !allocated_dst_buf) {
        printf("Memory allocation failed\n");
        if (src_buf) free(src_buf);
        if (allocated_dst_buf) free(allocated_dst_buf);
        return -1;
    }

    // 读取源图像数据
    if (0 != read_image_from_file(src_buf, path, src_width, src_height, src_format, 0)) {
        printf("src image read err\n");
        memset(src_buf, 0xaa, src_buf_size);
    }
    memset(allocated_dst_buf, 0x80, dst_buf_size);

    // RGA处理
    src_handle = importbuffer_virtualaddr(src_buf, src_buf_size);
    dst_handle = importbuffer_virtualaddr(allocated_dst_buf, dst_buf_size);
    if (src_handle == 0 || dst_handle == 0) {
        printf("importbuffer failed!\n");
        ret = -1;
        goto release_buffer;
    }

    src_img = wrapbuffer_handle(src_handle, src_width, src_height, src_format);
    dst_img = wrapbuffer_handle(dst_handle, dst_width, dst_height, dst_format);

    ret = imcheck(src_img, dst_img, {}, {});
    if (IM_STATUS_NOERROR != ret) {
        printf("%d, check error! %s\n", __LINE__, imStrError((IM_STATUS)ret));
        ret = -1;
        goto release_buffer;
    }

    ret = imcvtcolor(src_img, dst_img, src_format, dst_format);
    if (ret == IM_STATUS_SUCCESS) {
        printf("%s imcvtcolor running success!\n", path);
        // 返回分配的内存指针
        *dst_buf = allocated_dst_buf;
        *dst_size = dst_buf_size;
        allocated_dst_buf = nullptr;  // 防止被释放
        ret = 0;
    } else {
        printf("%s imcvtcolor running failed, %s\n", path, imStrError((IM_STATUS)ret));
        ret = -1;
    }

release_buffer:
    if (src_handle) releasebuffer_handle(src_handle);
    if (dst_handle) releasebuffer_handle(dst_handle);
    if (src_buf) free(src_buf);
    if (allocated_dst_buf) free(allocated_dst_buf);  // 只有失败时才释放

    return ret;
}