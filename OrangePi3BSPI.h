#pragma once
#include <cstdint>
#include <string>
#include "ISPIInterface.h"

class OrangePi3BSPI : public ISPIInterface {
public:
    OrangePi3BSPI(uint8_t channel, uint32_t speed = 500000, uint8_t mode = 0, uint8_t bits_per_word = 8);
    ~OrangePi3BSPI() override;

    bool init() override;
    void close() override;
    void setSpeed(uint32_t speed) override;
    void setMode(uint8_t mode) override;
    void send(const std::vector<uint8_t>& data) override;
    void recv(std::vector<uint8_t>& data, size_t len) override;
private:
    int spi_fd; // SPI设备文件描述符
    uint8_t channel; // SPI通道号
    uint32_t speed; // SPI速度
    uint8_t mode; // SPI模式
    uint8_t bits_per_word; // 每字传输位数
};