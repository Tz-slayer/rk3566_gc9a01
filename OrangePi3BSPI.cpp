#include "OrangePi3BSPI.h"
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <unistd.h>

OrangePi3BSPI::OrangePi3BSPI(uint8_t channel, uint32_t port, uint32_t speed, uint8_t mode, uint8_t bits_per_word)
    : channel(channel), port(port), speed(speed), mode(mode), bits_per_word(bits_per_word) {}

OrangePi3BSPI::~OrangePi3BSPI() {
    close();
}

bool OrangePi3BSPI::init() {
    spi_fd = wiringPiSPISetupMode(channel, port, speed, mode);
    if (spi_fd < 0) {
        return false;
    }

    return true;
}

void OrangePi3BSPI::close() {
    if (spi_fd >= 0) {
        ::close(spi_fd);
        spi_fd = -1;
    }
}

void OrangePi3BSPI::send(const std::vector<uint8_t>& data) {
    if (spi_fd < 0) {
        printf("SPI not initialized\n");
        return;
    }

    std::vector<uint8_t> buffer = data; // Create a mutable copy
    if (wiringPiSPIDataRW(channel, buffer.data(), buffer.size()) == -1) {
        printf("SPI send failed\n");
        return;
    }
}

void OrangePi3BSPI::recv(std::vector<uint8_t>& data, size_t len) {
    if (spi_fd < 0) {
        printf("SPI not initialized\n");
        return;
    }

    data.resize(len);
    if (wiringPiSPIDataRW(channel, data.data(), len) == -1) {
        printf("SPI receive failed\n");
        return;
    }
}
