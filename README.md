# 类结构图

```mermaid
classDiagram
    class IDriverDisplay {
        <<interface>>
        +init(direction=2): void
        +sendMemory(data): void
        +setWindow(x0, y0, x1, y1): void
        +getWidth(): uint16_t
        +getHeight(): uint16_t
    }

    class GC9A01Display {
        -spi_interface: ISPIInterface*
        -gpio_interface: IGPIOInterface*
        -width: uint16_t
        -height: uint16_t
        +init(direction=2): void
        +sendMemory(data): void
        +setWindow(x0, y0, x1, y1): void
        +getWidth(): uint16_t
        +getHeight(): uint16_t
    }

    class ISPIInterface {
        <<interface>>
        +init(): bool
        +close(): void
        +send(data): void
        +recv(data, len): void
    }

    class IGPIOInterface {
        <<interface>>
        +init(): bool
        +close(): void
        +setPin(pin, value): void
        +getPin(pin): bool
        +setDC(isData): void
    }

    class IDrawStrategy {
        <<interface>>
        +clear(display, color): void
        +displayImage(display, image, region): void
    }

    class BasicDrawStrategy {
        +clear(display, color): void
        +displayImage(display, image, region): void
    }

    class OrangePi3BSPI {
        -spi_fd: int
        -channel: uint8_t
        -speed: uint32_t
        +init(): bool
        +send(data): void
        +recv(data, len): void
    }

    class OrangePi3BGPIO {
        -LED: uint8_t
        -DC: uint8_t
        -RST: uint8_t
        -CS: uint8_t
        +init(): bool
        +setPin(pin, value): void
        +setDC(isData): void
    }

    IDriverDisplay <|.. GC9A01Display : 实现
    GC9A01Display --> ISPIInterface : 聚合
    GC9A01Display --> IGPIOInterface : 聚合
    ISPIInterface <|.. OrangePi3BSPI : 实现
    IGPIOInterface <|.. OrangePi3BGPIO : 实现
    IDrawStrategy <|.. BasicDrawStrategy : 实现
    BasicDrawStrategy --> IDriverDisplay : 策略依赖驱动接口