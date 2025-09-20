#pragma once
#include <string>
#include <cstdint>
#include <linux/spi/spidev.h> 
#include "ISPIInterface.h"

struct WiringPiSPIParams {
    int channel;    // SPI通道号
    int port;       // SPI端口号
    int speed;      // 传输速率
    int mode;       // SPI模式
};

class WiringPiSPI: public ISPIInterface {
public:
    WiringPiSPI(WiringPiSPIParams params, uint8_t led, uint8_t dc, uint8_t rst, uint8_t cs, uint8_t sda, uint8_t scl, uint8_t vcc = -1, uint8_t gnd = -1);
    ~WiringPiSPI();

    // 打开SPI设备并设置参数
    bool open(int channel, int port, int speed, int mode = 0);

    // 关闭SPI设备
    void close(int channel);

    // 读写数据（全双工）
    int dataRW(int channel, uint8_t* data, int len);

    // 获取文件描述符
    int getFd(int channel) const;

private:
    static const int MAX_CHANNEL = 7;
    int spiFds[MAX_CHANNEL];
    WiringPiSPIParams params;
    uint32_t spiSpeeds[MAX_CHANNEL];

    // 实现CommInterface接口
    void writeReg(uint8_t reg) override;
    void writeData(uint8_t data) override;
    void sendData(const std::vector<uint8_t>& data) override;
};