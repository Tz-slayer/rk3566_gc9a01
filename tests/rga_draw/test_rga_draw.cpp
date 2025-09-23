#include "../../GC9A01Display.h"
#include "../../BasicDrawStrategy.h"
#include "../../OrangePi3BGPIO.h"
#include "../../OrangePi3BSPI.h"
#include "../../DMAPool.h"
#include "../../RGAImageProcessor.h"
#include <iostream>
#include <vector>
#include <unistd.h>
#include <csignal>
#include <atomic>
#include <chrono>
#include <opencv2/opencv.hpp>

// 全局退出标志
volatile std::atomic<bool> exitFlag(false);

// 全局信号处理
void signalHandler(int signal) {
    std::cerr << "Received signal " << signal << ", exiting..." << std::endl;
    exitFlag.store(true);
}

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
    dma_pool.release(dst_dma_buf);
    dst_dma_buf = rga_processor.resize(dst_dma_buf->va, 480, 480, 240, 240, RK_FORMAT_RGB_565, RK_FORMAT_RGB_565);
    drawStrategy.displayImage((uint16_t*)dst_dma_buf->va, {0, 0, 239, 239});
    dma_pool.print_status();
    dma_pool.release(dst_dma_buf);
    dma_pool.print_status();

    usleep(1000000); // 等待1秒
}

// TODO：BasicDrawStrategy::displayImageSequence函数对于rga没有进行适配，导致dma内存空间无法在显示一张图像之后释放掉资源腾出dma池空间
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
    if (!dma_pool.preallocate(2 * 1024 * 1024, 4)) {
        std::cout << "DMA Pool preallocate failed" << std::endl;
    }
    DMABuffer *dst_dma_buf;
    RGAImageProcessor rga_processor(dma_pool);

    char imagePath1[256];
    while (!exitFlag.load()) {  // 外层循环检查退出标志
        for (int i = 0; i < 14 && !exitFlag.load(); i++) {  // 内层也检查
            sprintf(imagePath1, "../../images/test2/test2_frame_%d.webp", i + 1);
            cv::Mat src_image = cv::imread(imagePath1);
            if (src_image.empty()) continue;

            dst_dma_buf = rga_processor.cvtColor(src_image.data, 480, 480, RK_FORMAT_RGB_888, RK_FORMAT_RGB_565);
            if (dst_dma_buf) {
                dma_pool.release(dst_dma_buf);
                dst_dma_buf = rga_processor.resize(dst_dma_buf->va, 480, 480, 240, 240, RK_FORMAT_RGB_565, RK_FORMAT_RGB_565);
                if (dst_dma_buf) {
                    drawStrategy.displayImage((uint16_t*)dst_dma_buf->va, {0, 0, 239, 239});
                    dma_pool.release(dst_dma_buf);
                }
            }
            usleep(70000);
        }
    }
}

int main() {
    // 注册信号处理
    std::signal(SIGSEGV, signalHandler);
    std::signal(SIGABRT, signalHandler);
    std::signal(SIGINT, signalHandler);  // Ctrl+C

    try {
        test_displayImage();
        test_displayImageSequence();
    } catch (const std::exception& e) {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
    }

    return 0;
}