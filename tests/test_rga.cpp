#include "RGAImageProcessor.h"
#include "utils/PerformanceTester.h"
#include <opencv2/opencv.hpp>
#include "dma_alloc.h"
#include "DMAPool.h"
#include "im2d.h"
#include "RgaUtils.h"
#include <iostream>
#include <vector>
#include <cstdio>

DMAPool dma_pool;

// 使用opencv测试cvTcolor
int test_cvtColor_opencv() {
    std::string webp_file = "../images/test2/test2_frame_1.webp";
    // 读取图像
    cv::Mat src_img = cv::imread(webp_file);
    cv::Mat dst_img;
    cv::cvtColor(src_img, dst_img, cv::COLOR_BGR2RGB);
    return 0;
}

// 使用rga测试cvTcolor
int test_cvtColor_rga() {
    std::string webp_file = "../images/test2/test2_frame_1.webp";

    // 读取图像
    cv::Mat src_img = cv::imread(webp_file);
    int width = src_img.cols, height = src_img.rows;
    int src_format = RK_FORMAT_BGR_888;  // OpenCV 默认是 BGR
    int dst_format = RK_FORMAT_RGB_565;

    RGAImageProcessor rga_processor(dma_pool);
    void *dst_image = rga_processor.cvtColor(src_img.data, width, height, src_format, dst_format);
    if (!dst_image) {
        std::cout << "RGA color conversion failed" << std::endl;
        return -1;
    }
    return 0;
}

// 使用opencv测试resize
int test_resize_opencv() {
    std::string webp_file = "../images/test2/test2_frame_1.webp";
    // 读取图像
    cv::Mat src_img = cv::imread(webp_file);
    int src_width = src_img.cols, src_height = src_img.rows;
    int dst_width = src_width / 2;   // 目标宽度减半
    int dst_height = src_height / 2; // 目标高度减半
    cv::Mat dst_img;
    cv::resize(src_img, dst_img, cv::Size(dst_width, dst_height));
    return 0;
}

// 使用rga测试resize
int test_resize_rga() {
    std::string webp_file = "../images/test2/test2_frame_1.webp";

    // 读取图像
    cv::Mat src_img = cv::imread(webp_file);
    int src_width = src_img.cols, src_height = src_img.rows;
    int src_format = RK_FORMAT_BGR_888;  // OpenCV 默认是 BGR
    int dst_format = RK_FORMAT_RGB_888;

    int dst_width = src_width / 2;   // 目标宽度减半
    int dst_height = src_height / 2; // 目标高度减半

    RGAImageProcessor rga_processor(dma_pool);
    void *dst_image = rga_processor.resize(src_img.data, src_width, src_height, dst_width, dst_height, src_format, dst_format);
    if (!dst_image) {
        std::cout << "RGA resize failed" << std::endl;
        return -1;
    }
    return 0;
}

int main() {
    PerformanceTester tester;
    // 提前分配好 DMA 内存空间
    if (!dma_pool.preallocate(2 * 1024 * 1024, 4)) {
        std::cout << "Failed to preallocate DMA buffers" << std::endl;
        return -1;
    }
    tester.runMultipleTimes(test_cvtColor_opencv, 100, "OpenCV Color Conversion");
    tester.runMultipleTimes(test_cvtColor_rga, 100, "RGA Color Conversion");
    tester.runMultipleTimes(test_resize_opencv, 100, "OpenCV Resize");
    tester.runMultipleTimes(test_resize_rga, 100, "RGA Resize");

    return 0;
}