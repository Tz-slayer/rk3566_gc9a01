#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <chrono>  // 添加计时库
#include <iostream> // 添加输出流库

int main() {
    int spi_fd = open("/dev/spidev3.0", O_RDWR);

    // 设置速率（示例：32MHz，需硬件支持）
    uint32_t speed = 1000 * 1000 * 40;
    ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);

    // 传输数据
    unsigned char tx_data[4096] = {0}; // 示例发送缓冲区
    unsigned char rx_data[4096] = {0}; // 示例接收缓冲区
    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx_data,  // 发送缓冲区地址
        .rx_buf = (unsigned long)rx_data,  // 接收缓冲区地址
        .len = 4096,                       // 数据长度
        .speed_hz = speed,
    };
    // 测量 sendMemory 时间
    auto start = std::chrono::high_resolution_clock::now();
    ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr);  // 一次系统调用完成传输
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "sendMemory time: " << duration.count() << " ms" << std::endl;

    close(spi_fd);
    return 0;
}