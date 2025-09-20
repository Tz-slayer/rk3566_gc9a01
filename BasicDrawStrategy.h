#pragma once
#include <cstdint>
#include "IDrawStrategy.h"
#include "GC9A01Display.h"

class BasicDrawStrategy : public IDrawStrategy {
private:
    GC9A01Display* display; // 指向GC9A01Display实例的指针

public:
    BasicDrawStrategy(GC9A01Display* disp) : display(disp) {}
    ~BasicDrawStrategy() override = default;

}