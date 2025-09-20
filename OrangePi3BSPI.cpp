#include "OrangePi3BSPI.h"
#include <wiringPiSPI.h>

OrangePi3BSPI::OrangePi3BSPI(uint8_t channel, uint32_t speed, uint8_t mode, uint8_t bits_per_word)
    : channel(channel), speed(speed), mode(mode), bits_per_word(bits_per_word) {}

OrangePi3BSPI::~OrangePi3BSPI() {
    close();
}

bool OrangePi3BSPI::init() {
    if (wiringPiSPISetup(channel, speed) == -1) {
        return false;
    }

    spi_fd = ::open(device.c_str(), O_RDWR);
    if (spi_fd < 0) {
        return false;
    }

    if (ioctl(spi_fd, SPI_IOC_WR_MODE, &mode) < 0) {
        ::close(spi_fd);
        spi_fd = -1;
        return false;
    }
    if (ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word) < 0) {
        ::close(spi_fd);
        spi_fd = -1;
        return false;
    }
    if (ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
        ::close(spi_fd);
        spi_fd = -1;
        return false;
    }

    return true;
}
