#pragma once
#include <chrono>
#include <iostream>
#include <iomanip>
#include <string>

// 性能测试工具类，用于测量代码执行时间
class PerformanceTester {
private:
    // 高精度时钟类型
    using Clock = std::chrono::high_resolution_clock;
    // 时间点类型
    using TimePoint = std::chrono::time_point<Clock>;
    // 纳秒级时间间隔
    using DurationNs = std::chrono::nanoseconds;

    TimePoint start_time;       // 开始时间
    TimePoint end_time;         // 结束时间
    bool is_running = false;    // 测试是否正在进行

public:
    // 开始计时
    void start();

    // 结束计时
    void stop();

    // 获取耗时（纳秒）
    uint64_t getDurationNs() const;

    // 获取耗时（微秒）
    double getDurationUs() const;

    // 获取耗时（毫秒）
    double getDurationMs() const;

    // 获取耗时（秒）
    double getDurationS() const;

    // 打印结果（自动选择合适的单位）
    void printResult(const std::string& test_name = "Code Segment") const;

    // 多次测试并取平均值（模板函数，必须在头文件中定义）
    template <typename Func>
    void runMultipleTimes(Func&& func, size_t iterations, const std::string& test_name = "Code Segment") {
        if (iterations == 0) {
            std::cout << "Error: Number of iterations cannot be 0" << std::endl;
            return;
        }

        // 预热（避免首次执行的额外开销影响结果）
        func();

        // 正式测试
        start();
        for (size_t i = 0; i < iterations; ++i) {
            func();
        }
        stop();

        // 计算单次平均时间
        uint64_t total_ns = getDurationNs();
        double avg_ns = total_ns / (double)iterations;

        std::cout << "[" << test_name << "] Executed " << iterations << " times, ";
        std::cout << "Total time: " << total_ns / 1e6 << " ms, ";
        std::cout << "Average time per run: ";

        if (avg_ns >= 1e9) {
            std::cout << std::fixed << std::setprecision(3) << avg_ns / 1e9 << " s";
        } else if (avg_ns >= 1e6) {
            std::cout << std::fixed << std::setprecision(3) << avg_ns / 1e6 << " ms";
        } else if (avg_ns >= 1e3) {
            std::cout << std::fixed << std::setprecision(3) << avg_ns / 1e3 << " us";
        } else {
            std::cout << std::fixed << std::setprecision(0) << avg_ns << " ns";
        }
        std::cout << std::endl;
    }
};