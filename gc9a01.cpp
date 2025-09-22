#include "gc9a01.h"


void GC9A01::columnAddressSet(uint16_t y0, uint16_t y1) {
    comm->writeReg(0x2A);  // 列地址设置
    comm->writeData((y0 >> 8) & 0xFF);
    comm->writeData(y0 & 0xFF);
    comm->writeData((y1 >> 8) & 0xFF);
    comm->writeData(y1 & 0xFF);
}

void GC9A01::rowAddressSet(uint16_t x0, uint16_t x1) {
    comm->writeReg(0x2B);  // 行地址设置
    comm->writeData((x0 >> 8) & 0xFF);
    comm->writeData(x0 & 0xFF);
    comm->writeData((x1 >> 8) & 0xFF);
    comm->writeData(x1 & 0xFF);
}

void GC9A01::memeoryWrite() {
    comm->writeReg(0x2C);  // 准备接收数据
}

// 私有函数：设置显示窗口
void GC9A01::setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    columnAddressSet(y0, y1);
    rowAddressSet(x0, x1);
    memeoryWrite();
}

// 初始化显示屏
void GC9A01::init(uint8_t direction=2) {
    comm->writeReg(0xEF);
    comm->writeReg(0xEB);
    comm->writeData(0x14);

    comm->writeReg(0xFE);
    comm->writeReg(0xEF);

    comm->writeReg(0xEB);
    comm->writeData(0x14);

    comm->writeReg(0x84);
    comm->writeData(0x40);

    comm->writeReg(0x85);
    comm->writeData(0xFF);

    comm->writeReg(0x86);
    comm->writeData(0xFF);

    comm->writeReg(0x87);
    comm->writeData(0xFF);

    comm->writeReg(0x88);
    comm->writeData(0x0A);

    comm->writeReg(0x89);
    comm->writeData(0x21);

    comm->writeReg(0x8A);
    comm->writeData(0x00);

    comm->writeReg(0x8B);
    comm->writeData(0x80);

    comm->writeReg(0x8C);
    comm->writeData(0x01);

    comm->writeReg(0x8D);
    comm->writeData(0x01);

    comm->writeReg(0x8E);
    comm->writeData(0xFF);

    comm->writeReg(0x8F);
    comm->writeData(0xFF);

    comm->writeReg(0xB6);
    comm->writeData(0x00);
    comm->writeData(0x20);

    // 内存扫描方向，也就是更改显示方向
    comm->writeReg(0x36);
    if(direction==0) comm->writeData(0x08);
    else if(direction==1) comm->writeData(0xC8);
    else if(direction==2) comm->writeData(0x68);
    else comm->writeData(0xA8);

    comm->writeReg(0x3A);
    comm->writeData(0x05);

    comm->writeReg(0x90);
    comm->writeData(0x08);
    comm->writeData(0x08);
    comm->writeData(0x08);
    comm->writeData(0x08);

    comm->writeReg(0xBD);
    comm->writeData(0x06);

    comm->writeReg(0xBC);
    comm->writeData(0x00);

    comm->writeReg(0xFF);
    comm->writeData(0x60);
    comm->writeData(0x01);
    comm->writeData(0x04);

    comm->writeReg(0xC3);
    comm->writeData(0x13);
    comm->writeReg(0xC4);
    comm->writeData(0x13);

    comm->writeReg(0xC9);
    comm->writeData(0x22);

    comm->writeReg(0xBE);
    comm->writeData(0x11);

    comm->writeReg(0xE1);
    comm->writeData(0x10);
    comm->writeData(0x0E);

    comm->writeReg(0xDF);
    comm->writeData(0x21);
    comm->writeData(0x0c);
    comm->writeData(0x02);


    comm->writeReg(0xF0);
    comm->writeData(0x45);
    comm->writeData(0x09);
    comm->writeData(0x08);
    comm->writeData(0x08);
    comm->writeData(0x26);
    comm->writeData(0x2A);

    comm->writeReg(0xF1);
    comm->writeData(0x43);
    comm->writeData(0x70);
    comm->writeData(0x72);
    comm->writeData(0x36);
    comm->writeData(0x37);
    comm->writeData(0x6F);

    comm->writeReg(0xF2);
    comm->writeData(0x45);
    comm->writeData(0x09);
    comm->writeData(0x08);
    comm->writeData(0x08);
    comm->writeData(0x26);
    comm->writeData(0x2A);

    comm->writeReg(0xF3);
    comm->writeData(0x43);
    comm->writeData(0x70);
    comm->writeData(0x72);
    comm->writeData(0x36);
    comm->writeData(0x37);
    comm->writeData(0x6F);

    comm->writeReg(0xED);
    comm->writeData(0x1B);
    comm->writeData(0x0B);

    comm->writeReg(0xAE);
    comm->writeData(0x77);

    comm->writeReg(0xCD);
    comm->writeData(0x63);

    comm->writeReg(0x70);
    comm->writeData(0x07);
    comm->writeData(0x07);
    comm->writeData(0x04);
    comm->writeData(0x0E);
    comm->writeData(0x0F);
    comm->writeData(0x09);
    comm->writeData(0x07);
    comm->writeData(0x08);
    comm->writeData(0x03);

    comm->writeReg(0xE8);
    comm->writeData(0x34);

    comm->writeReg(0x62);
    comm->writeData(0x18);
    comm->writeData(0x0D);
    comm->writeData(0x71);
    comm->writeData(0xED);
    comm->writeData(0x70);
    comm->writeData(0x70);
    comm->writeData(0x18);
    comm->writeData(0x0F);
    comm->writeData(0x71);
    comm->writeData(0xEF);
    comm->writeData(0x70);
    comm->writeData(0x70);

    comm->writeReg(0x63);
    comm->writeData(0x18);
    comm->writeData(0x11);
    comm->writeData(0x71);
    comm->writeData(0xF1);
    comm->writeData(0x70);
    comm->writeData(0x70);
    comm->writeData(0x18);
    comm->writeData(0x13);
    comm->writeData(0x71);
    comm->writeData(0xF3);
    comm->writeData(0x70);
    comm->writeData(0x70);

    comm->writeReg(0x64);
    comm->writeData(0x28);
    comm->writeData(0x29);
    comm->writeData(0xF1);
    comm->writeData(0x01);
    comm->writeData(0xF1);
    comm->writeData(0x00);
    comm->writeData(0x07);

    comm->writeReg(0x66);
    comm->writeData(0x3C);
    comm->writeData(0x00);
    comm->writeData(0xCD);
    comm->writeData(0x67);
    comm->writeData(0x45);
    comm->writeData(0x45);
    comm->writeData(0x10);
    comm->writeData(0x00);
    comm->writeData(0x00);
    comm->writeData(0x00);

    comm->writeReg(0x67);
    comm->writeData(0x00);
    comm->writeData(0x3C);
    comm->writeData(0x00);
    comm->writeData(0x00);
    comm->writeData(0x00);
    comm->writeData(0x01);
    comm->writeData(0x54);
    comm->writeData(0x10);
    comm->writeData(0x32);
    comm->writeData(0x98);

    comm->writeReg(0x74);
    comm->writeData(0x10);
    comm->writeData(0x85);
    comm->writeData(0x80);
    comm->writeData(0x00);
    comm->writeData(0x00);
    comm->writeData(0x4E);
    comm->writeData(0x00);

    comm->writeReg(0x98);
    comm->writeData(0x3E);
    comm->writeData(0x07);

    comm->writeReg(0x35);
    comm->writeReg(0x21);

    comm->writeReg(0x11);
    comm->writeReg(0x29);
}

// 清屏
void GC9A01::clear(uint16_t color) {
    setWindow(0, 0, width-1, height-1);
    std::vector<uint8_t> data;
    data.reserve(width * height * 2);
    uint8_t high = (color >> 8) & 0xFF;
    uint8_t low = color & 0xFF;
    for (uint32_t i = 0; i < width * height; ++i) {
        data.push_back(high);
        data.push_back(low);
    }
    comm->sendData(data);
}

// 显示RGB565格式图片
void GC9A01::displayImage(const uint16_t* image, std::vector<uint16_t> region) {
    setWindow(region[0], region[1], region[2], region[3]);
    // 转换RGB565数组为字节流并发送
    std::vector<uint8_t> data;
    data.reserve(width * height * 2);
    for (uint32_t i = 0; i < width * height; ++i) {
        data.push_back((image[i] >> 8) & 0xFF);
        data.push_back(image[i] & 0xFF);
    }
    comm->sendData(data);
}
