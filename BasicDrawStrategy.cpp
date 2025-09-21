#include "BasicDrawStrategy.h"

void BasicDrawStrategy::sendInChunks(const std::vector<uint8_t>& data, size_t chunkSize) {
    size_t totalSize = data.size();
    for (size_t offset = 0; offset < totalSize; offset += chunkSize) {
        size_t end = std::min(offset + chunkSize, totalSize);
        std::vector<uint8_t> chunk(data.begin() + offset, data.begin() + end);
        display->sendMemory(chunk);
    }
}

void BasicDrawStrategy::clear(uint16_t color) {
    uint16_t width = display->getWidth();
    uint16_t height = display->getHeight();
    display->setWindow(0, 0, width - 1, height - 1);
    size_t totalPixels = static_cast<size_t>(width) * height;
    size_t bufferSize = totalPixels * 2;  // 每个像素2字节

    // 预计算颜色字节
    uint8_t highByte = static_cast<uint8_t>((color >> 8) & 0xFF);
    uint8_t lowByte = static_cast<uint8_t>(color & 0xFF);

    // 创建完整缓冲区
    std::vector<uint8_t> buffer(bufferSize);
    for (size_t i = 0; i < bufferSize; i += 2) {
        buffer[i] = highByte;
        buffer[i + 1] = lowByte;
    }

    // 使用分块发送
    sendInChunks(buffer, 4096);  // 块大小4096字节
}

void BasicDrawStrategy::displayImage(const uint16_t* image, std::vector<uint16_t> region) {
    if (region.size() != 4) return; // 确保区域参数正确

    uint16_t x0 = region[0];
    uint16_t y0 = region[1];
    uint16_t x1 = region[2];
    uint16_t y1 = region[3];

    display->setWindow(x0, y0, x1, y1);
    size_t width = x1 - x0 + 1;
    size_t height = y1 - y0 + 1;
    size_t totalPixels = width * height;
    size_t bufferSize = totalPixels * 2; // 每个像素2字节

    // 创建完整缓冲区
    std::vector<uint8_t> buffer(bufferSize);
    for (size_t i = 0; i < totalPixels; ++i) {
        buffer[2 * i] = static_cast<uint8_t>((image[i] >> 8) & 0xFF);     // 高字节
        buffer[2 * i + 1] = static_cast<uint8_t>(image[i] & 0xFF);        // 低字节
    }

    // 使用分块发送
    sendInChunks(buffer, 4096); // 块大小4096字节
}