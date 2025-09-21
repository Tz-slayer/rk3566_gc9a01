#include "GC9A01Display.h"
#include <unistd.h>
#include <cstdio>

void GC9A01Display::sendMemory(const std::vector<uint8_t>& data) {
    spi_interface->send(data);
}

uint16_t GC9A01Display::getWidth() const {
    return width;
}

uint16_t GC9A01Display::getHeight() const {
    return height;
}

void GC9A01Display::writeReg(uint8_t reg) {
    gpio_interface->setDC(false); // 命令模式
    spi_interface->send({reg});
    gpio_interface->setDC(true);  // 数据模式
}

void GC9A01Display::writeData(uint8_t data) {
    spi_interface->send({data});
}

void GC9A01Display::columnAddressSet(uint16_t y0, uint16_t y1) {
    writeReg(0x2A);  // 列地址设置
    writeData(static_cast<uint8_t>((y0 >> 8) & 0xFF));
    writeData(static_cast<uint8_t>(y0 & 0xFF));
    writeData(static_cast<uint8_t>((y1 >> 8) & 0xFF));
    writeData(static_cast<uint8_t>(y1 & 0xFF));
}

void GC9A01Display::rowAddressSet(uint16_t x0, uint16_t x1) {
    writeReg(0x2B);  // 行地址设置
    writeData(static_cast<uint8_t>((x0 >> 8) & 0xFF));
    writeData(static_cast<uint8_t>(x0 & 0xFF));
    writeData(static_cast<uint8_t>((x1 >> 8) & 0xFF));
    writeData(static_cast<uint8_t>(x1 & 0xFF));
}

void GC9A01Display::memeoryWrite() {
    writeReg(0x2C);  // 准备接收数据
}

// 私有函数：设置显示窗口
void GC9A01Display::setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    columnAddressSet(y0, y1);
    rowAddressSet(x0, x1);
    memeoryWrite();
}

// 初始化芯片
void GC9A01Display::init(uint8_t direction) {
    writeReg(0xEF);
    writeReg(0xEB);
    writeData(0x14);

    writeReg(0xFE);
    writeReg(0xEF);

    writeReg(0xEB);
    writeData(0x14);

    writeReg(0x84);
    writeData(0x40);

    writeReg(0x85);
    writeData(0xFF);

    writeReg(0x86);
    writeData(0xFF);

    writeReg(0x87);
    writeData(0xFF);

    writeReg(0x88);
    writeData(0x0A);

    writeReg(0x89);
    writeData(0x21);

    writeReg(0x8A);
    writeData(0x00);

    writeReg(0x8B);
    writeData(0x80);

    writeReg(0x8C);
    writeData(0x01);

    writeReg(0x8D);
    writeData(0x01);

    writeReg(0x8E);
    writeData(0xFF);

    writeReg(0x8F);
    writeData(0xFF);

    writeReg(0xB6);
    writeData(0x00);
    writeData(0x20);

    // 内存扫描方向，也就是更改显示方向
    writeReg(0x36);
    if(direction==0) writeData(0x08);
    else if(direction==1) writeData(0xC8);
    else if(direction==2) writeData(0x68);
    else writeData(0xA8);

    writeReg(0x3A);
    writeData(0x05);

    writeReg(0x90);
    writeData(0x08);
    writeData(0x08);
    writeData(0x08);
    writeData(0x08);

    writeReg(0xBD);
    writeData(0x06);

    writeReg(0xBC);
    writeData(0x00);

    writeReg(0xFF);
    writeData(0x60);
    writeData(0x01);
    writeData(0x04);

    writeReg(0xC3);
    writeData(0x13);
    writeReg(0xC4);
    writeData(0x13);

    writeReg(0xC9);
    writeData(0x22);

    writeReg(0xBE);
    writeData(0x11);

    writeReg(0xE1);
    writeData(0x10);
    writeData(0x0E);

    writeReg(0xDF);
    writeData(0x21);
    writeData(0x0c);
    writeData(0x02);


    writeReg(0xF0);
    writeData(0x45);
    writeData(0x09);
    writeData(0x08);
    writeData(0x08);
    writeData(0x26);
    writeData(0x2A);

    writeReg(0xF1);
    writeData(0x43);
    writeData(0x70);
    writeData(0x72);
    writeData(0x36);
    writeData(0x37);
    writeData(0x6F);

    writeReg(0xF2);
    writeData(0x45);
    writeData(0x09);
    writeData(0x08);
    writeData(0x08);
    writeData(0x26);
    writeData(0x2A);

    writeReg(0xF3);
    writeData(0x43);
    writeData(0x70);
    writeData(0x72);
    writeData(0x36);
    writeData(0x37);
    writeData(0x6F);

    writeReg(0xED);
    writeData(0x1B);
    writeData(0x0B);

    writeReg(0xAE);
    writeData(0x77);

    writeReg(0xCD);
    writeData(0x63);

    writeReg(0x70);
    writeData(0x07);
    writeData(0x07);
    writeData(0x04);
    writeData(0x0E);
    writeData(0x0F);
    writeData(0x09);
    writeData(0x07);
    writeData(0x08);
    writeData(0x03);

    writeReg(0xE8);
    writeData(0x34);

    writeReg(0x62);
    writeData(0x18);
    writeData(0x0D);
    writeData(0x71);
    writeData(0xED);
    writeData(0x70);
    writeData(0x70);
    writeData(0x18);
    writeData(0x0F);
    writeData(0x71);
    writeData(0xEF);
    writeData(0x70);
    writeData(0x70);

    writeReg(0x63);
    writeData(0x18);
    writeData(0x11);
    writeData(0x71);
    writeData(0xF1);
    writeData(0x70);
    writeData(0x70);
    writeData(0x18);
    writeData(0x13);
    writeData(0x71);
    writeData(0xF3);
    writeData(0x70);
    writeData(0x70);

    writeReg(0x64);
    writeData(0x28);
    writeData(0x29);
    writeData(0xF1);
    writeData(0x01);
    writeData(0xF1);
    writeData(0x00);
    writeData(0x07);

    writeReg(0x66);
    writeData(0x3C);
    writeData(0x00);
    writeData(0xCD);
    writeData(0x67);
    writeData(0x45);
    writeData(0x45);
    writeData(0x10);
    writeData(0x00);
    writeData(0x00);
    writeData(0x00);

    writeReg(0x67);
    writeData(0x00);
    writeData(0x3C);
    writeData(0x00);
    writeData(0x00);
    writeData(0x00);
    writeData(0x01);
    writeData(0x54);
    writeData(0x10);
    writeData(0x32);
    writeData(0x98);

    writeReg(0x74);
    writeData(0x10);
    writeData(0x85);
    writeData(0x80);
    writeData(0x00);
    writeData(0x00);
    writeData(0x4E);
    writeData(0x00);

    writeReg(0x98);
    writeData(0x3E);
    writeData(0x07);

    writeReg(0x35);
    writeReg(0x21);

    writeReg(0x11);
    writeReg(0x29);
}

void GC9A01Display::reSet() {
    gpio_interface->setRST(false);
    usleep(50000); // 50ms
    gpio_interface->setRST(true);
    usleep(50000); // 50ms
}

// 初始化显示屏
void GC9A01Display::initDisplay(uint8_t direction) {
    gpio_interface->setDC(true);
    gpio_interface->setRST(true);
    gpio_interface->setCS(false);
    gpio_interface->setLED(true);
    reSet();
    init(direction);
    printf("Display initialized\n");
}