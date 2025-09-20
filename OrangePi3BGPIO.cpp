#include "OrangePi3BGPIO.h"
#include <wiringPi.h>

OrangePi3BGPIO::~OrangePi3BGPIO() {
    close();
}

bool OrangePi3BGPIO::init() {
    if (wiringPiSetupGpio() == -1) {
        return false;
    }

    pinMode(LED, OUTPUT);
    pinMode(DC, OUTPUT);
    pinMode(RST, OUTPUT);
    pinMode(CS, OUTPUT);
    pinMode(SDA, OUTPUT);
    pinMode(SCL, OUTPUT);

    if (VCC != static_cast<uint8_t>(-1)) {
        pinMode(VCC, OUTPUT);
        digitalWrite(VCC, HIGH); // 供电
    }
    if (GND != static_cast<uint8_t>(-1)) {
        pinMode(GND, OUTPUT);
        digitalWrite(GND, LOW); // 接地
    }

    digitalWrite(CS, LOW); // 片选低电平
    digitalWrite(LED, HIGH); // 背光开启

    return true;
}

void OrangePi3BGPIO::close() {
    digitalWrite(LED, LOW); // 关闭背光
    digitalWrite(CS, HIGH); // 片选高电平

    if (VCC != static_cast<uint8_t>(-1)) {
        digitalWrite(VCC, LOW); // 断电
    }
}

void OrangePi3BGPIO::setPin(uint8_t pin, bool value) {
    digitalWrite(pin, value ? HIGH : LOW);
}

bool OrangePi3BGPIO::getPin(uint8_t pin) const {
    return digitalRead(pin) == HIGH;
}

void OrangePi3BGPIO::setPinMode(uint8_t pin, uint8_t mode) {
    pinMode(pin, mode);
}