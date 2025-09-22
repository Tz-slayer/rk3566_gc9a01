#include "RGAImageProcessor.h"
#include "utils/PerformanceTester.h"
#include <opencv2/opencv.hpp>
#include "dma_alloc.h"
#include "im2d.h"
#include "RgaUtils.h"
#include <iostream>
#include <vector>
#include <cstdio>

int test_cvtColor() {
    DMAPool dma_pool;
    // 预分配4个2MB的DMA缓冲区
    if (!dma_pool.preallocate(2 * 1024 * 1024, 4)) {
        std::cout << "Failed to preallocate DMA buffers" << std::endl;
        return -1;
    }

    dma_pool.print_status();

    std::string webp_file = "../images/test2/test2_frame_1.webp";

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

int test_resize() {
    DMAPool dma_pool;
    // 预分配4个8MB的DMA缓冲区
    if (!dma_pool.preallocate(8 * 1024 * 1024, 4)) {
        std::cout << "Failed to preallocate DMA buffers" << std::endl;
        return -1;
    }

    dma_pool.print_status();

    std::string webp_file = "../images/test2/test2_frame_1.webp";

    // 读取图像
    cv::Mat src_img = cv::imread(webp_file);
    int src_width = src_img.cols, src_height = src_img.rows;
    int src_format = RK_FORMAT_BGR_888;  // OpenCV 默认是 BGR
    int dst_format = RK_FORMAT_RGB_888;

    int dst_width = src_width / 2;   // 目标宽度减半
    int dst_height = src_height / 2; // 目标高度减半

    RGAImageProcessor rga_processor(dma_pool);
    int ret = rga_processor.resize(src_img.data, nullptr, src_width, src_height, dst_width, dst_height, src_format, dst_format);
    if (ret != 0) {
        std::cout << "RGA resize failed" << std::endl;
        return -1;
    }

    dma_pool.print_status();
    std::cout << "✓ RGA conversion successful!" << std::endl;
    return 0;
}

int main() {
    PerformanceTester tester;
    tester.runMultipleTimes(test_cvtColor, 10, "RGA Color Conversion");
    tester.runMultipleTimes(test_resize, 10, "RGA Resize");
    return 0;
}