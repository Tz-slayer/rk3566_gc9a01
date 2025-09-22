# DMA 池

```mermaid
classDiagram
    class DMABuffer {
        +int fd          // DMA 文件描述符
        +void* va        // 虚拟地址
        +size_t size     // 缓冲区大小
        +bool in_use     // 是否正在使用
    }

    class DMAPool {
        -std::vector~DMABuffer~ pool_    // DMA 缓冲区池
        -std::mutex mutex_               // 互斥锁
        -const char* heap_path_          // DMA 堆路径
        
        +DMAPool(const char* heap_path)  // 构造函数
        +~DMAPool()                      // 析构函数
        +bool preallocate(size_t buffer_size, int count)  // 预分配缓冲区
        +DMABuffer* acquire(size_t required_size)         // 获取可用缓冲区
        +void release(DMABuffer* buf)                     // 释放缓冲区
        +void print_status()                              // 打印池状态
    }

    DMAPool --> DMABuffer : 管理