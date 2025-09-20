cmake_minimum_required(VERSION 3.10)
project(OrangePiGPIO_Test)

set(CMAKE_CXX_STANDARD 11)

# 添加源文件
add_executable(test_gpio
    test_gpio.cpp
    ../OrangePi3BGPIO.cpp
)

# 包含头文件目录（如有需要可补充父目录）
target_include_directories(test_gpio PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ../)

# 链接必要的系统库
target_link_libraries(test_gpio PRIVATE pthread wiringPi)