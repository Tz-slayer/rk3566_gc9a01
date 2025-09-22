#include "../GC9A01Display.h"
#include "../BasicDrawStrategy.h"
#include "../OrangePi3BGPIO.h"
#include "../OrangePi3BSPI.h"
#include <iostream>
#include <vector>
#include <unistd.h>
#include <chrono>  // 添加计时库
#include <opencv2/opencv.hpp>

int main() {
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
        return 1;
    }

    // 创建 SPI 对象
    OrangePi3BSPI spi(3, 0, 1000 * 1000 * 40);
    if (!spi.init()) {
        std::cout << "SPI init failed" << std::endl;
        return 1;
    }

    // 创建显示对象（假设 240x240 屏幕）
    GC9A01Display display(&spi, &gpio, 240, 240);

    // 创建绘制策略
    BasicDrawStrategy drawStrategy(&display);

    // 测试初始化
    std::cout << "Testing init..." << std::endl;
    display.initDisplay(2);  // 默认方向

    // 测试设置窗口
    std::cout << "Testing setWindow..." << std::endl;
    display.setWindow(0, 0, 239, 239);

    // 测试绘制策略（清屏）
    std::cout << "Testing draw strategy clear..." << std::endl;
    drawStrategy.clear(0xFFFF);  // 清屏为白色

    usleep(1000000); // 等待1秒

    // 使用 OpenCV 读取图像并显示
    std::cout << "Testing displayImage with OpenCV..." << std::endl;
    std::string imagePath = "../images/test1.jpg";  // 替换为实际图像路径
    cv::Mat img = cv::imread(imagePath);
    if (img.empty()) {
        std::cout << "Failed to load image: " << imagePath << std::endl;
        return 1;
    }

    // 调整图像大小到 240x240（如果需要）
    cv::resize(img, img, cv::Size(240, 240));

    // 转换为 RGB（OpenCV 默认 BGR）
    cv::cvtColor(img, img, cv::COLOR_BGR2RGB);

    // 转换为 RGB565 格式
    std::vector<uint16_t> image(240 * 240);
    for (int y = 0; y < 240; ++y) {
        for (int x = 0; x < 240; ++x) {
            cv::Vec3b pixel = img.at<cv::Vec3b>(y, x);
            uint8_t r = pixel[0] >> 3;  // R5
            uint8_t g = pixel[1] >> 2;  // G6
            uint8_t b = pixel[2] >> 3;  // B5
            uint16_t rgb565 = (r << 11) | (g << 5) | b;
            image[y * 240 + x] = rgb565;
        }
    }

    std::vector<uint16_t> region = {0, 0, 239, 239};  // 全屏区域
    drawStrategy.displayImage(image.data(), region);

    usleep(1000000); // 等待1秒

    std::vector<cv::Mat> imgs;
    char imagePath1[256];

    for (int i = 0; i < 14; i++) {
        sprintf(imagePath1, "../images/test2/test2_frame_%d.webp", i + 1);
        cv::Mat img = cv::imread(imagePath1);
        if (!img.empty()) {
            imgs.push_back(img.clone());  // 避免覆盖
        }
    }

    while (true) {
        for (const auto& img : imgs) {
            // 转换并显示
            cv::Mat resizedImg;
            cv::resize(img, resizedImg, cv::Size(240, 240));
            cv::cvtColor(resizedImg, resizedImg, cv::COLOR_BGR2RGB);
            std::vector<uint16_t> imgData(240 * 240);
            // 转换为 RGB565 格式
            for (int y = 0; y < 240; ++y) {
                for (int x = 0; x < 240; ++x) {
                    cv::Vec3b pixel = resizedImg.at<cv::Vec3b>(y, x);
                    uint8_t r = pixel[0] >> 3;  // R5
                    uint8_t g = pixel[1] >> 2;  // G6
                    uint8_t b = pixel[2] >> 3;  // B5
                    uint16_t rgb565 = (r << 11) | (g << 5) | b;
                    imgData[y * 240 + x] = rgb565;
                }
            }
            drawStrategy.displayImage(imgData.data(), region);
            usleep(100 * 1000);
        }
    }
    

    // 测试 sendMemory 时间（可选）
    std::cout << "Testing sendMemory time..." << std::endl;
    std::vector<uint8_t> chunk(4096, 0xFF);  // 示例 chunk
    auto start = std::chrono::high_resolution_clock::now();
    display.sendMemory(chunk);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "sendMemory time: " << duration.count() << " ms" << std::endl;

    usleep(1000000); // 等待1秒

    std::cout << "Test completed." << std::endl;
    return 0;
}