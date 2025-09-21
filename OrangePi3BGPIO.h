#pragma once
#include "IGPIOInterface.h"

class OrangePi3BGPIO : public IGPIOInterface {
public:
    OrangePi3BGPIO(uint8_t led, uint8_t dc, uint8_t rst, uint8_t cs, uint8_t sda, uint8_t scl, uint8_t vcc = -1, uint8_t gnd = -1)
        : LED(led), DC(dc), RST(rst), CS(cs), SDA(sda), SCL(scl), VCC(vcc), GND(gnd) {};
    ~OrangePi3BGPIO() override;

    // 初始化GPIO接口
    bool init() override;
    // 关闭GPIO接口
    void close() override;
    // 设置GPIO引脚状态
    void setPin(uint8_t pin, bool value) override;
    // 获取GPIO引脚状态
    bool getPin(uint8_t pin) const override;
    // 设置GPIO引脚模式
    void setPinMode(uint8_t pin, uint8_t mode) override;
    // 设置DC
    void setDC(bool isData) override;
    // 设置RST
    void setRST(bool isReset) override;
    // 设置CS
    void setCS(bool isSelect) override;
    // 设置LED
    void setLED(bool isOn) override;
private:
    uint8_t LED;
    uint8_t DC;
    uint8_t RST;
    uint8_t CS;
    uint8_t SDA;
    uint8_t SCL;
    uint8_t VCC;
    uint8_t GND;

};