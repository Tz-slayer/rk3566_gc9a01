#include "../../GC9A01Display.h"
#include "../../BasicDrawStrategy.h"
#include "../../OrangePi3BGPIO.h"
#include "../../OrangePi3BSPI.h"
#include "../../DMAPool.h"
#include "../../RGAImageProcessor.h"
#include <iostream>
#include <vector>
#include <unistd.h>
#include <chrono>  // 添加计时库
#include <opencv2/opencv.hpp>

void test_displayImage() {
    uint8_t LED = 10;
    uint8_t DC = 8;
    uint8_t RST = 5;
    uint8_t CS = 7;
    uint8_t SDA = 11;
    uint8_t SCL = 12;
    int src_width = 480, src_height = 480;

    std::string imagePath = "../../images/test2/test2_frame_1.webp";  // 替换为实际图像路径

    // 创建 GPIO 对象（参数根据硬件调整）
    OrangePi3BGPIO gpio(LED, DC, RST, CS, SDA, SCL);
    if (!gpio.init()) {  // 假设有 init() 方法
        std::cout << "GPIO init failed" << std::endl;
    }
    // 创建 SPI 对象
    OrangePi3BSPI spi(3, 0, 1000 * 1000 * 40);
    if (!spi.init()) {
        std::cout << "SPI init failed" << std::endl;
    }
    // 创建显示对象（假设 240x240 屏幕）
    GC9A01Display display(&spi, &gpio, 240, 240);
    // 创建绘制策略
    BasicDrawStrategy drawStrategy(&display);
    // 测试初始化
    // std::cout << "Testing init..." << std::endl;
    display.initDisplay(2);  // 默认方向
    DMAPool dma_pool;
    if (!dma_pool.preallocate(2 * 1024 * 1024, 4)) {
        std::cout << "DMA Pool preallocate failed" << std::endl;
    }
    cv::Mat src_image = cv::imread(imagePath);
    DMABuffer *dst_dma_buf;
    RGAImageProcessor rga_processor(dma_pool);
    dst_dma_buf = rga_processor.cvtColor(src_image.data, src_width, src_height, RK_FORMAT_RGB_888, RK_FORMAT_RGB_565);
    dst_dma_buf = rga_processor.resize(dst_dma_buf->va, 480, 480, 240, 240, RK_FORMAT_RGB_565, RK_FORMAT_RGB_565);
    drawStrategy.displayImage((uint16_t*)dst_dma_buf->va, {0, 0, 239, 239});
    dma_pool.release(dst_dma_buf);

    usleep(1000000); // 等待1秒
}

void test_displayImageSequence() {
    uint8_t LED = 10;
    uint8_t DC = 8;
    uint8_t RST = 5;
    uint8_t CS = 7;
    uint8_t SDA = 11;
    uint8_t SCL = 12;

    // 创建 GPIO 对象（参数根据硬件调整）
    OrangePi3BGPIO gpio(LED, DC, RST, CS, SDA, SCL);
    if (!gpio.init()) {  // 假设有 init() 方法
        std::cout << "GPIO init failed" << std::endl;
    }
    // 创建 SPI 对象
    OrangePi3BSPI spi(3, 0, 1000 * 1000 * 40);
    if (!spi.init()) {
        std::cout << "SPI init failed" << std::endl;
    }
    // 创建显示对象（假设 240x240 屏幕）
    GC9A01Display display(&spi, &gpio, 240, 240);
    // 创建绘制策略
    BasicDrawStrategy drawStrategy(&display);
    // 测试初始化
    display.initDisplay(2);  // 默认方向
    DMAPool dma_pool;
    if (!dma_pool.preallocate(2 * 1024 * 1024, 32)) {
        std::cout << "DMA Pool preallocate failed" << std::endl;
    }
    DMABuffer *dst_dma_buf;
    RGAImageProcessor rga_processor(dma_pool);
    std::vector<DMABuffer*> dma_bufs;
    char imagePath1[256];
    for (int i = 0; i < 14; i++) {
        sprintf(imagePath1, "../../images/test2/test2_frame_%d.webp", i + 1);
        cv::Mat src_image = cv::imread(imagePath1);
        dst_dma_buf = rga_processor.cvtColor(src_image.data, 480, 480, RK_FORMAT_RGB_888, RK_FORMAT_RGB_565);
        dst_dma_buf = rga_processor.resize(dst_dma_buf->va, 480, 480, 240, 240, RK_FORMAT_RGB_565, RK_FORMAT_RGB_565);
        dma_bufs.push_back(dst_dma_buf);
    }
    std::vector<std::vector<uint16_t>> regions;
    for (int i = 0; i < 14; ++i) {
        regions.push_back({0, 0, 239, 239}); // 全屏区域
    }
    drawStrategy.displayImageSequence(
        std::vector<uint16_t*>{(uint16_t*)dma_bufs[0]->va, (uint16_t*)dma_bufs[1]->va, (uint16_t*)dma_bufs[2]->va, (uint16_t*)dma_bufs[3]->va,
                               (uint16_t*)dma_bufs[4]->va, (uint16_t*)dma_bufs[5]->va, (uint16_t*)dma_bufs[6]->va, (uint16_t*)dma_bufs[7]->va,
                               (uint16_t*)dma_bufs[8]->va, (uint16_t*)dma_bufs[9]->va, (uint16_t*)dma_bufs[10]->va, (uint16_t*)dma_bufs[11]->va,
                               (uint16_t*)dma_bufs[12]->va, (uint16_t*)dma_bufs[13]->va},
        regions,
        80);
    for (auto buf : dma_bufs) {
        dma_pool.release(buf);
    }
}

int main() {
    test_displayImage();
    test_displayImageSequence();
    return 0;
}