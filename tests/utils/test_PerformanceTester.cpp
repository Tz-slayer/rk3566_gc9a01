#include "PerformanceTester.h"

void myFunction() {
    // 要测量的代码
    for (int i = 0; i < 1000; ++i) {
        // some code
    }
}

int main() {
    PerformanceTester tester;

    // 开始计时
    tester.start();

    // 要测量的代码（例如，循环或函数调用）
    for (int i = 0; i < 1000000; ++i) {
        // some code
    }

    // 结束计时
    tester.stop();

    // 打印结果
    tester.printResult("Loop Test");

    // 或者手动获取时间
    uint64_t ns = tester.getDurationNs();
    double ms = tester.getDurationMs();
    std::cout << "Duration: " << ms << " ms" << std::endl;

    // 运行 myFunction 10 次并取平均
    tester.runMultipleTimes(myFunction, 10, "My Function Test");

    return 0;
}