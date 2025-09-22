cmake_minimum_required(VERSION 3.10)
project(OrangePi_Test)  # 修改项目名称以包含GPIO和SPI测试

set(CMAKE_CXX_STANDARD 11)
find_package(OpenCV REQUIRED)

# 添加GPIO测试可执行文件
add_executable(test_gpio
    test_gpio.cpp
    ../OrangePi3BGPIO.cpp
)

# 添加SPI测试可执行文件
add_executable(test_spi
    test_spi.cpp
    ../OrangePi3BSPI.cpp
)

# 添加DRAW测试可执行文件
add_executable(test_draw
    test_draw.cpp
    ../BasicDrawStrategy.cpp
    ../GC9A01Display.cpp
    ../OrangePi3BGPIO.cpp
    ../OrangePi3BSPI.cpp
)

add_executable(test_performance
    utils/test_PerformanceTester.cpp
    ../utils/PerformanceTester.cpp
)

# 添加RGA测试可执行文件
add_executable(test_rga
    test_rga.cpp
    /usr/local/src/dma_alloc.cpp
    ../RGAImageProcessor.cpp
    ../utils/PerformanceTester.cpp
    ../DMAPool.cpp
)

# 包含头文件目录
target_include_directories(test_gpio PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ../
                            /usr/local/include)
target_include_directories(test_spi PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ../
                            /usr/local/include)
target_include_directories(test_draw PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ../
                            /usr/local/include
                            ${OpenCV_INCLUDE_DIRS})
target_include_directories(test_performance PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ../
                            /usr/local/include ../utils)
target_include_directories(test_rga PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ../
                            /usr/local/include ../utils ${OpenCV_INCLUDE_DIRS})

# 链接必要的系统库
target_link_libraries(test_gpio PRIVATE pthread wiringPi)
target_link_libraries(test_spi PRIVATE wiringPi)
target_link_libraries(test_draw PRIVATE wiringPi ${OpenCV_LIBS})
target_link_libraries(test_performance PRIVATE pthread)
target_link_libraries(test_rga PRIVATE pthread ${OpenCV_LIBS} rga)