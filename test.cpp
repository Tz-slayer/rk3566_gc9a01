#include "utils/rga.h"
#include <opencv2/opencv.hpp>
#include "dma_alloc.h"
#include "im2d.h"
#include "RgaUtils.h"
#include <iostream>
#include <vector>
#include <cstdio>

#include "RGAImageProcessor.h"

int main() {
    DMAPool dma_pool;
    // 预分配4个2MB的DMA缓冲区
    if (!dma_pool.preallocate(2 * 1024 * 1024, 4)) {
        std::cout << "Failed to preallocate DMA buffers" << std::endl;
        return -1;
    }

    std::string webp_file = "../tests/images/test2/test2_frame_1.webp";

    // 读取图像
    cv::Mat src_img = cv::imread(webp_file);
    int width = src_img.cols, height = src_img.rows;
    int src_format = RK_FORMAT_BGR_888;  // OpenCV 默认是 BGR
    int dst_format = RK_FORMAT_RGB_565;

    RGAImageProcessor rga_processor(dma_pool);
    int ret = rga_processor.cvtColor(src_img.data, nullptr, width, height, src_format, dst_format);
    if (ret != 0) {
        std::cout << "RGA color conversion failed" << std::endl;
        return -1;
    }
    return 0;
}

// int main() {
//     std::string webp_file = "../tests/images/test2/test2_frame_1.webp";

//     // 读取图像
//     cv::Mat src_img = cv::imread(webp_file);
//     if (src_img.empty()) {
//         std::cout << "Failed to load image" << std::endl;
//         return -1;
//     }
    
//     int width = src_img.cols, height = src_img.rows;
    
//     std::cout << "Src_img type: " << src_img.type() << std::endl;
//     std::cout << "Src_img size: " << src_img.size() << std::endl;
    
//     // 修正：使用正确的格式和大小
//     int src_format = RK_FORMAT_BGR_888;  // OpenCV 默认是 BGR
//     size_t src_buf_size = width * height * get_bpp_from_format(src_format);  // 3 bytes per pixel
//     std::cout << "Src_buf size: " << src_buf_size << " bytes (format: " << src_format << ")" << std::endl;
    
//     // 方案1：使用 DMA 内存（推荐）
//     int src_dma_fd;
//     void* src_dma_va;
//     int ret = dma_buf_alloc(DMA_HEAP_DMA32_UNCACHED_PATH, src_buf_size, &src_dma_fd, &src_dma_va);
//     if (ret < 0) {
//         printf("alloc dma32_heap buffer for source failed!\n");
//         return -1;
//     }
    
//     // 复制数据到 DMA 内存
//     memcpy(src_dma_va, src_img.data, src_buf_size);
    
//     // 导入 DMA 缓冲区到 RGA
//     rga_buffer_handle_t src_handle = importbuffer_fd(src_dma_fd, src_buf_size);
//     if (src_handle == 0) {
//         printf("import src dma_fd error!\n");
//         dma_buf_free(src_buf_size, &src_dma_fd, src_dma_va);
//         return -1;
//     }
    
//     rga_buffer_t src = wrapbuffer_handle(src_handle, width, height, src_format);
    
//     // 计算目标图像缓冲区大小
//     int dst_format = RK_FORMAT_RGB_565;
//     size_t dst_buf_size = width * height * get_bpp_from_format(dst_format);
    
//     // 创建目标图像DMA缓冲区
//     int dst_dma_fd;
//     void* dst_dma_va;
//     ret = dma_buf_alloc(DMA_HEAP_DMA32_UNCACHED_PATH, dst_buf_size, &dst_dma_fd, &dst_dma_va);
//     if (ret < 0) {
//         printf("alloc dma32_heap buffer for destination failed!\n");
//         dma_buf_free(src_buf_size, &src_dma_fd, src_dma_va);
//         return -1;
//     }
    
//     // 将DMA描述符导入RGA
//     rga_buffer_handle_t dst_handle = importbuffer_fd(dst_dma_fd, dst_buf_size);
//     if (dst_handle == 0) {
//         printf("import dst dma_fd error!\n");  
//         dma_buf_free(src_buf_size, &src_dma_fd, src_dma_va);
//         dma_buf_free(dst_buf_size, &dst_dma_fd, dst_dma_va);
//         return -1;
//     } 
    
//     // 将句柄封装成完整的RGA缓冲区结构
//     rga_buffer_t dst = wrapbuffer_handle(dst_handle, width, height, dst_format);
    
//     std::cout << "=== RGA Color Conversion Test ===" << std::endl;
//     std::cout << "Input size: " << width << "x" << height << std::endl;
//     std::cout << "Output buffer size: " << dst_buf_size << " bytes" << std::endl;
    
//     // 检查缓冲区兼容性
//     ret = imcheck(src, dst, {}, {});
//     if (IM_STATUS_NOERROR != ret) {
//         printf("imcheck failed: %s\n", imStrError((IM_STATUS)ret));
//         dma_buf_free(src_buf_size, &src_dma_fd, src_dma_va);
//         dma_buf_free(dst_buf_size, &dst_dma_fd, dst_dma_va);
//         return -1;
//     }
    
//     // 转换颜色格式
//     ret = imcvtcolor(src, dst, src_format, dst_format);
//     if (ret != IM_STATUS_SUCCESS) {
//         printf("imcvtcolor failed with code: %d, %s\n", ret, imStrError((IM_STATUS)ret));
//         dma_buf_free(src_buf_size, &src_dma_fd, src_dma_va);
//         dma_buf_free(dst_buf_size, &dst_dma_fd, dst_dma_va);
//         return -1;
//     }
    
//     std::cout << "✓ RGA conversion successful!" << std::endl;
    
//     // 验证结果（打印前几个像素）
//     uint16_t* rgb565_data = (uint16_t*)dst_dma_va;
//     std::cout << "First 5 RGB565 pixels: ";
//     for (int i = 0; i < 5; ++i) {
//         uint16_t pixel = rgb565_data[i];
//         uint8_t r = (pixel >> 11) & 0x1F;
//         uint8_t g = (pixel >> 5) & 0x3F;
//         uint8_t b = pixel & 0x1F;
//         std::cout << "0x" << std::hex << pixel << "(R:" << std::dec << (int)r 
//                   << ",G:" << (int)g << ",B:" << (int)b << ") ";
//     }
//     std::cout << std::endl;
    
//     // 保存结果
//     FILE* out_file = fopen("/tmp/rga_result.bin", "wb");
//     if (out_file) {
//         fwrite(dst_dma_va, dst_buf_size, 1, out_file);
//         fclose(out_file);
//         std::cout << "✓ Result saved to /tmp/rga_result.bin" << std::endl;
//     }
    
//     // 释放内存
//     dma_buf_free(src_buf_size, &src_dma_fd, src_dma_va);
//     dma_buf_free(dst_buf_size, &dst_dma_fd, dst_dma_va);
    
//     std::cout << "\n=== Test Complete ===" << std::endl;
//     return 0;
// }