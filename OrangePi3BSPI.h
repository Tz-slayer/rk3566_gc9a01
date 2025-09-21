#pragma once
#include <cstdint>
#include <string>
#include <cstddef>
#include "ISPIInterface.h"

class OrangePi3BSPI : public ISPIInterface {
public:
    OrangePi3BSPI(uint8_t channel, uint32_t port, uint32_t speed = 500000, uint8_t mode = 0, uint8_t bits_per_word = 8);
    ~OrangePi3BSPI() override;

    bool init() override;
    void close() override;
    void send(const std::vector<uint8_t>& data) override;
    void recv(std::vector<uint8_t>& data, size_t len) override;
private:
    int spi_fd = -1;
    uint8_t channel;
    uint32_t port;
    uint32_t speed;
    uint8_t mode;
    uint8_t bits_per_word;
};