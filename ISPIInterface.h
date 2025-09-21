#pragma once
#include <cstdint>
#include <cstddef>
#include <vector>

// SPI通信接口抽象类（桥接模式的Implementor）
class ISPIInterface {
public:
    ISPIInterface() {}
    virtual ~ISPIInterface() = default;
    // 初始化SPI接口
    virtual bool init() = 0;
    // 关闭SPI接口
    virtual void close() = 0;
    // 设置SPI模式
    virtual void send(const std::vector<uint8_t>& data) = 0;
    virtual void recv(std::vector<uint8_t>& data, size_t len) = 0;
};
