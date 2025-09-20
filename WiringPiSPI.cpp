#include "wiringPiSPI.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <cstring>
#include <cstdio>
#include <cerrno>
#include "wiringOP/wiringPi/wiringPi.h"

WiringPiSPI::WiringPiSPI(WiringPiSPIParams params, uint8_t led, uint8_t dc, uint8_t rst, uint8_t cs, uint8_t sda, uint8_t scl, uint8_t vcc, uint8_t gnd)
    : CommInterface(led, dc, rst, cs, sda, scl, vcc, gnd), params(params) {
    for (int i = 0; i < MAX_CHANNEL; ++i) {
        spiFds[i] = -1;
        spiSpeeds[i] = 0;
    }
}

WiringPiSPI::~WiringPiSPI() {
    for (int i = 0; i < MAX_CHANNEL; ++i) {
        if (spiFds[i] != -1) {
            ::close(spiFds[i]);
            spiFds[i] = -1;
        }
    }
}

bool WiringPiSPI::open(int channel, int port, int speed, int mode) {
    if (channel < 0 || channel >= MAX_CHANNEL) return false;
    char spidev[20];
    snprintf(spidev, sizeof(spidev), "/dev/spidev%d.%d", channel, port);
    printf("Opening device %s\n", spidev);

    int fd = ::open(spidev, O_RDWR);
    if (fd < 0) {
        printf("Unable to open SPI device: %s\n", strerror(errno));
        return false;
    }

    spiSpeeds[channel] = speed;
    spiFds[channel] = fd;

    uint8_t spiBPW = 8;
    uint16_t spiDelay = 0;

    if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0) {
        printf("SPI Mode Change failure: %s\n", strerror(errno));
        ::close(fd);
        spiFds[channel] = -1;
        return false;
    }
    if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &spiBPW) < 0) {
        printf("SPI BPW Change failure: %s\n", strerror(errno));
        ::close(fd);
        spiFds[channel] = -1;
        return false;
    }
    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
        printf("SPI Speed Change failure: %s\n", strerror(errno));
        ::close(fd);
        spiFds[channel] = -1;
        return false;
    }

    return true;
}

void WiringPiSPI::close(int channel) {
    if (channel < 0 || channel >= MAX_CHANNEL) return;
    if (spiFds[channel] != -1) {
        ::close(spiFds[channel]);
        spiFds[channel] = -1;
    }
}

int WiringPiSPI::dataRW(int channel, uint8_t* data, int len) {
    if (channel < 0 || channel >= MAX_CHANNEL || spiFds[channel] == -1) return -1;
    struct spi_ioc_transfer spi;
    memset(&spi, 0, sizeof(spi));
    spi.tx_buf = (unsigned long)data;
    spi.rx_buf = (unsigned long)data;
    spi.len = len;
    spi.delay_usecs = 0;
    spi.speed_hz = spiSpeeds[channel];
    spi.bits_per_word = 8;
    return ioctl(spiFds[channel], SPI_IOC_MESSAGE(1), &spi);
}

int WiringPiSPI::getFd(int channel) const {
    if (channel < 0 || channel >= MAX_CHANNEL) return -1;
    return spiFds[channel];
}

// 实现CommInterface接口
void WiringPiSPI::writeReg(uint8_t reg) {
    digitalWrite(DC, LOW); // 写寄存器
    dataRW(params.channel, &reg, 1); // 发送数据
    digitalWrite(DC, HIGH); // 切换回数据模式
}

void WiringPiSPI::writeData(uint8_t data) {
    dataRW(params.channel, &data, 1);
}

void WiringPiSPI::sendData(const std::vector<uint8_t>& data) {
    for (const auto& byte : data) {
        writeData(byte);
    }
}
