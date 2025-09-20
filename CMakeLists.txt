cmake_minimum_required(VERSION 3.10)
project(WiringPiSPI_Test)

set(CMAKE_CXX_STANDARD 11)

# 添加源文件
add_executable(test
    test.cpp
    wiringPiSPI.cpp
)

# 包含头文件目录
target_include_directories(test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# 链接必要的系统库
target_link_libraries(test PRIVATE pthread wiringPi)