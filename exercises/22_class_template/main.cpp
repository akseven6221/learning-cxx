#include "../exercise.h"
#include <cstring>
// READ: 类模板 <https://zh.cppreference.com/w/cpp/language/class_template>

template<class T>
struct Tensor4D {
    unsigned int shape[4];
    T *data;

    Tensor4D(unsigned int const shape_[4], T const *data_) {
    unsigned int size = 1;
    for (int i = 0; i < 4; i++) {
        shape[i] = shape_[i];
        size *= shape[i];    // 计算总元素个数
    }
    data = new T[size];      // 分配内存
    std::memcpy(data, data_, size * sizeof(T));  // 复制数据
}

    ~Tensor4D() {
        delete[] data;
    }

    // For simplicity, disallow copying and moving
    Tensor4D(Tensor4D const &) = delete;
    Tensor4D(Tensor4D &&) noexcept = delete;

    // Broadcasted addition
    Tensor4D &operator+=(Tensor4D const &others) {
    // 1. 检查形状是否兼容
    for (int i = 0; i < 4; i++) {
        if (others.shape[i] != shape[i] && others.shape[i] != 1) {
            throw std::invalid_argument("Incompatible shapes for broadcasting");
        }
    }

    // 2. 计算总元素数
    unsigned int size = 1;
    for (int i = 0; i < 4; i++) {
        size *= shape[i];
    }

    // 3. 遍历每个元素进行广播加法
    for (unsigned int i = 0; i < size; i++) {
        // 计算当前索引在各个维度上的位置
        unsigned int idx[4];
        unsigned int stride = 1;
        for (int j = 3; j >= 0; j--) {
            idx[j] = (i / stride) % shape[j];
            stride *= shape[j];
        }

        // 计算广播后的索引
        unsigned int otherIdx = 0;
        stride = 1;
        for (int j = 3; j >= 0; j--) {
            otherIdx += (others.shape[j] == 1 ? 0 : idx[j]) * stride;
            stride *= others.shape[j];
        }

        // 执行加法
        data[i] += others.data[otherIdx];
    }

    return *this;
}
};

// ---- Do not modify the code below ----
int main(int argc, char **argv) {
    {
        unsigned int shape[]{1, 2, 3, 4};
        int data[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,
            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        auto t0 = Tensor4D(shape, data);
        auto t1 = Tensor4D(shape, data);
        t0 += t1;
        for (unsigned int i = 0; i < sizeof(data) / sizeof(int); i++) {
            ASSERT(t0.data[i] == data[i] * 2, "Tensor doubled by plus its self.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        float d0[]{
            1, 1, 1, 1,
            2, 2, 2, 2,
            3, 3, 3, 3,
            4, 4, 4, 4,
            5, 5, 5, 5,
            6, 6, 6, 6};
        unsigned int s1[]{1, 2, 3, 1};
        float d1[]{
            6,
            5,
            4,
            3,
            2,
            1};

        auto t0 = Tensor4D(s0, d0);
        auto t1 = Tensor4D(s1, d1);
        t0 += t1;
        for (unsigned int i = 0; i < sizeof(d0) / sizeof(float); i++) {
            ASSERT(t0.data[i] == 7.f, "Every element of t0 should be 7 after adding t1 to it.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        double d0[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,
            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        unsigned int s1[]{1, 1, 1, 1};
        double d1[]{1};

        auto t0 = Tensor4D(s0, d0);
        auto t1 = Tensor4D(s1, d1);
        t0 += t1;
        for (unsigned int i = 0; i < sizeof(d0) / sizeof(double); i++) {
            ASSERT(t0.data[i] == d0[i] + 1, "Every element of t0 should be incremented by 1 after adding t1 to it.");
        }
    }
}
