#pragma once
#include "dma_alloc.h"
#include <vector>
#include <mutex>

struct DMABuffer {
    int fd;          // DMA 文件描述符
    void* va;        // 虚拟地址
    size_t size;    // 缓冲区大小
    bool in_use;    // 是否正在使用
};

class DMAPool {
private:
    std::vector<DMABuffer> pool_;
    std::mutex mutex_;
    const char* heap_path_;

public:
    DMAPool(const char* heap_path = DMA_HEAP_DMA32_UNCACHED_PATH) : heap_path_(heap_path) {}
    
    ~DMAPool() {
        for (auto& buf : pool_) {
            if (buf.fd >= 0) {
                dma_buf_free(buf.size, &buf.fd, buf.va);
            }
        }
    }
    
    // 预分配指定数量的缓冲区
    bool preallocate(size_t buffer_size, int count) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        for (int i = 0; i < count; ++i) {
            DMABuffer buf = { -1, nullptr, buffer_size, false };
            int ret = dma_buf_alloc(heap_path_, buffer_size, &buf.fd, &buf.va);
            if (ret < 0) {
                printf("Failed to preallocate DMA buffer %d\n", i);
                return false;
            }
            pool_.push_back(buf);
        }
        
        printf("Preallocated %d DMA buffers of %zu bytes each\n", count, buffer_size);
        return true;
    }
    
    // 获取可用缓冲区
    DMABuffer* acquire(size_t required_size) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        for (auto& buf : pool_) {
            if (!buf.in_use && buf.size >= required_size) {
                buf.in_use = true;
                return &buf;
            }
        }
        
        printf("No available DMA buffer for size %zu\n", required_size);
        return nullptr;
    }
    
    // 释放缓冲区
    void release(DMABuffer* buf) {
        if (buf) {
            std::lock_guard<std::mutex> lock(mutex_);
            buf->in_use = false;
        }
    }
    
    // 获取池状态
    void print_status() {
        std::lock_guard<std::mutex> lock(mutex_);
        int total = pool_.size();
        int used = 0;
        for (const auto& buf : pool_) {
            if (buf.in_use) used++;
        }
        printf("DMA Pool: %d/%d buffers in use\n", used, total);
    }
};