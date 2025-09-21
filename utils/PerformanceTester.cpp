#include "PerformanceTester.h"

// 开始计时
void PerformanceTester::start() {
    start_time = Clock::now();
    is_running = true;
}

// 结束计时
void PerformanceTester::stop() {
    if (is_running) {
        end_time = Clock::now();
        is_running = false;
    }
}

// 获取耗时（纳秒）
uint64_t PerformanceTester::getDurationNs() const {
    if (is_running) {
        return 0; // 若仍在运行，返回0表示无效
    }
    return std::chrono::duration_cast<DurationNs>(end_time - start_time).count();
}

// 获取耗时（微秒）
double PerformanceTester::getDurationUs() const {
    return getDurationNs() / 1000.0;
}

// 获取耗时（毫秒）
double PerformanceTester::getDurationMs() const {
    return getDurationUs() / 1000.0;
}

// 获取耗时（秒）
double PerformanceTester::getDurationS() const {
    return getDurationMs() / 1000.0;
}

// 打印结果（自动选择合适的单位）
void PerformanceTester::printResult(const std::string& test_name) const {
    if (is_running) {
        std::cout << "Error: Please call stop() first to end the test" << std::endl;
        return;
    }

    uint64_t ns = getDurationNs();
    std::cout << "[" << test_name << "] Execution time: ";

    if (ns >= 1e9) {
        std::cout << std::fixed << std::setprecision(3) << ns / 1e9 << " s";
    } else if (ns >= 1e6) {
        std::cout << std::fixed << std::setprecision(3) << ns / 1e6 << " ms";
    } else if (ns >= 1e3) {
        std::cout << std::fixed << std::setprecision(3) << ns / 1e3 << " us";
    } else {
        std::cout << ns << " ns";
    }
    std::cout << std::endl;
}