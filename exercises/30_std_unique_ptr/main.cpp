#include <iostream>
#include <vector>
#include <memory>
#include <cstring>
#include <cassert>

// 全局变量，用于记录 Resource 对象的生命周期
std::vector<std::string> RECORDS;

// Resource 类
class Resource {
    std::string _records;

public:
    void record(char record) {
        _records.push_back(record);
    }

    ~Resource() {
        RECORDS.push_back(_records);
    }
};

// 别名定义
using Unique = std::unique_ptr<Resource>;

// 操作函数
Unique reset(Unique ptr) {
    if (ptr) ptr->record('r');
    return std::make_unique<Resource>();
}

Unique drop(Unique ptr) {
    if (ptr) ptr->record('d');
    return nullptr;
}

Unique forward(Unique ptr) {
    if (ptr) ptr->record('f');
    return ptr;
}

// 主函数
int main(int argc, char **argv) {
    std::vector<std::string> problems[3];

    // 问题 0
    drop(forward(reset(nullptr)));
    problems[0] = std::move(RECORDS);

    // 问题 1
    forward(drop(reset(forward(forward(reset(nullptr))))));
    problems[1] = std::move(RECORDS);

    // 问题 2
    drop(drop(reset(drop(reset(reset(nullptr))))));
    problems[2] = std::move(RECORDS);

    // ---- 不要修改以上代码 ----

    // 答案数组
    std::vector<const char *> answers[]{
        {"fd"},          // 问题 0
        {"ffr", "d"},    // 问题 1
        {"r", "d", "d"}, // 问题 2
    };

    // ---- 不要修改以下代码 ----

    // 验证答案
    for (auto i = 0; i < 3; ++i) {
        assert(problems[i].size() == answers[i].size()); // 检查大小是否一致
        for (auto j = 0; j < problems[i].size(); ++j) {
            assert(std::strcmp(problems[i][j].c_str(), answers[i][j]) == 0); // 检查内容是否一致
        }
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}