#include "wiringPiSPI.h"
#include <iostream>
#include <cstring>

int main() {
    const int DC = 130;
    const int RST = 118;
    const int CS = 128;
    const int LED = 129;
    const int SDA = 138;
    const int SCL = 139;
    
    // 配置SPI参数
    WiringPiSPIParams params;
    params.channel = 3;
    params.port = 0;
    params.speed = 500000; // 500kHz
    params.mode = SPI_MODE_0; // SPI模式0

    WiringPiSPI spi(params, LED, DC, RST, CS, SDA, SCL);

    // 打开SPI设备
    if (!spi.open(params.channel, params.port, params.speed, params.mode)) {
        std::cerr << "Failed to open SPI device!" << std::endl;
        return 1;
    }
    std::cout << "SPI device opened successfully." << std::endl;

    // 构造测试数据
    uint8_t txData[4] = {0xAA, 0xBB, 0xCC, 0xDD};
    std::cout << "Before transfer: ";
    for (int i = 0; i < 4; ++i) std::cout << std::hex << (int)txData[i] << " ";
    std::cout << std::endl;

    // 进行数据读写
    int ret = spi.dataRW(params.channel, txData, 4);
    if (ret < 0) {
        std::cerr << "SPI transfer failed!" << std::endl;
        spi.close(params.channel);
        return 1;
    }

    std::cout << "After transfer: ";
    for (int i = 0; i < 4; ++i) std::cout << std::hex << (int)txData[i] << " ";
    std::cout << std::endl;

    // 获取文件描述符
    int fd = spi.getFd(params.channel);
    std::cout << "SPI fd: " << fd << std::endl;

    // 关闭SPI设备
    spi.close(params.channel);
    std::cout << "SPI device closed." << std::endl;

    return 0;
}