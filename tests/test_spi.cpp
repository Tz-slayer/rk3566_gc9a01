#include "../OrangePi3BSPI.h"
#include <iostream>
#include <vector>

int main() {
    // 参数请根据实际硬件配置调整
    uint8_t channel = 3;
    uint32_t port = 0;
    uint32_t speed = 500000;
    uint8_t mode = 0;
    uint8_t bits_per_word = 8;

    OrangePi3BSPI spi(channel, port, speed, mode, bits_per_word);

    if (!spi.init()) {
        std::cout << "SPI init failed" << std::endl;
        return 1;
    }

    // 发送测试数据
    std::vector<uint8_t> send_data = {0xAA, 0x55, 0xFF, 0x00};
    spi.send(send_data);

    // 接收测试数据
    std::vector<uint8_t> recv_data;
    spi.recv(recv_data, 4);

    std::cout << "Received data: ";
    for (auto val : recv_data) {
        std::cout << std::hex << static_cast<int>(val) << " ";
    }
    std::cout << std::endl;

    spi.close();
    return 0;
}