cmake_minimum_required(VERSION 3.10)
project(test_rga_draw)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # 强制使用 C++11
set(CMAKE_CXX_STANDARD_EXTENSIONS OFF)  # 禁用编译器扩展，确保标准兼容

find_package(OpenCV REQUIRED)
if(NOT OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV not found. Please install OpenCV.")
endif()

# 添加编译选项以提高健壮性
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")  # 启用所有警告，pedantic 模式，警告视为错误
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -fsanitize=address -fsanitize=undefined")  # 调试模式：调试符号、无优化、地址/未定义行为检查
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")  # 发布模式：高优化、禁用断言

# 检查平台特定选项（针对 RK3566/ARM）
if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv8-a -mtune=cortex-a55")  # ARM64 优化
endif()

# 添加静态分析（可选，需要 clang-tidy）
# find_program(CLANG_TIDY_EXE clang-tidy)
# if(CLANG_TIDY_EXE)
#     set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}" "-checks=*,-clang-analyzer-alpha.*,-llvm-include-order")
# endif()

add_executable(test_rga_draw
    test_rga_draw.cpp
    ../../RGAImageProcessor.cpp
    ../../BasicDrawStrategy.cpp
    ../../GC9A01Display.cpp
    ../../OrangePi3BGPIO.cpp
    ../../OrangePi3BSPI.cpp
    ../../DMAPool.cpp
    /usr/local/src/dma_alloc.cpp
    ../../utils/PerformanceTester.cpp
)

target_include_directories(test_rga_draw PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR} 
    ../../
    /usr/local/include 
    ../../utils
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(test_rga_draw PRIVATE 
    pthread 
    wiringPi 
    ${OpenCV_LIBS} 
    rga
)

# 添加 Valgrind 内存检查测试（仅在调试模式下）
find_program(VALGRIND_EXE valgrind)
if(VALGRIND_EXE AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_test(NAME test_rga_draw_valgrind 
             COMMAND ${VALGRIND_EXE} --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 $<TARGET_FILE:test_rga_draw>)
    set_tests_properties(test_rga_draw_valgrind PROPERTIES 
                         ENVIRONMENT "CK_FORK=no"  # 避免 Valgrind 与 CTest 的 fork 冲突
                         LABELS "valgrind")
endif()