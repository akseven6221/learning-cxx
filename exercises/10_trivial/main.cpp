#include "../exercise.h"

// READ: Trivial type <https://learn.microsoft.com/zh-cn/cpp/cpp/trivial-standard-layout-and-pod-types?view=msvc-170>

struct FibonacciCache {
    unsigned long long cache[16];
    int cached;
};

static unsigned long long fibonacci(FibonacciCache &cache, int i) {
    // 如果请求的数已经在缓存中，直接返回
    if (i < cache.cached) {
        return cache.cache[i];
    }
    
    // 从当前缓存的位置开始计算新的数
    int cached = cache.cached;
    // 计算直到达到所需的索引，同时确保不超过缓存数组大小
    while (cached <= i && cached < 16) {
        cache.cache[cached] = cache.cache[cached - 1] + cache.cache[cached - 2];
        cached++;
    }
    
    // 更新缓存计数
    cache.cached = cached;
    
    return cache.cache[i];
}

int main(int argc, char **argv) {
    // TODO: 初始化缓存结构体，使计算正确
    // NOTICE: C/C++ 中，读取未初始化的变量（包括结构体变量）是未定义行为
    // READ: 初始化的各种写法 <https://zh.cppreference.com/w/cpp/language/initialization>
    FibonacciCache fib = {{0, 1}, 2};
    ASSERT(fibonacci(fib, 10) == 55, "fibonacci(10) should be 55");
    std::cout << "fibonacci(10) = " << fibonacci(fib, 10) << std::endl;
    return 0;
}
