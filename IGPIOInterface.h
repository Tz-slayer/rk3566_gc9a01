#pragma once
#include <cstdint>
#include <vector>

// 通信接口抽象类（桥接模式的Implementor）
class IGPIOInterface {
public:
    IGPIOInterface(){}
    virtual ~IGPIOInterface() = default;
    // 初始化GPIO接口
    virtual bool init() = 0;
    // 关闭GPIO接口
    virtual void close() = 0;
    // 设置GPIO引脚状态
    virtual void setPin(uint8_t pin, bool value) = 0;
    // 获取GPIO引脚状态
    virtual bool getPin(uint8_t pin) const = 0;
    // 设置GPIO引脚模式
    virtual void setPinMode(uint8_t pin, uint8_t mode) = 0;
    // 设置DC
    virtual void setDC(bool isData) = 0;
    // 设置RST
    virtual void setRST(bool isReset) = 0;
    // 设置CS
    virtual void setCS(bool isSelect) = 0;
    // 设置LED
    virtual void setLED(bool isOn) = 0;
};

