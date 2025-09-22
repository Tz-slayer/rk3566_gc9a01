cmake_minimum_required(VERSION 3.10)
project(WiringPiSPI_Test)

set(CMAKE_CXX_STANDARD 11)

# 查找 OpenCV
find_package(OpenCV REQUIRED)
if(NOT OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV not found. Please install libopencv-dev.")
endif()

# 添加源文件
add_executable(test
    test.cpp
    # utils/rga.cpp
    /usr/local/src/dma_alloc.cpp
    # /usr/local/src/drm_alloc.cpp
    RGAImageProcessor.cpp
    DMAPool.cpp
)

# 包含头文件目录（添加 OpenCV 路径）
target_include_directories(test PRIVATE 
    /usr/local/include
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS}
)

# 链接必要的系统库（添加 OpenCV 库）
target_link_libraries(test PRIVATE 
    pthread 
    wiringPi
    ${OpenCV_LIBS}
    rga
)