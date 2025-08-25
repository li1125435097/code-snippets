#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 函数模板声明与定义（通常模板定义不能分离，需一起写出）
template <typename T>
void printVector(const vector<T>& vec) {
    cout << "[ ";
    for (const auto& item : vec) {
        cout << item << " ";
    }
    cout << "]" << endl;
}

template <typename T>
void print(const T& t) {
    cout << t << endl;
}

// vector连续数组存储，相较于普通数组支持动态扩容，尾插效率高，头插效率低
int main() {
    system("chcp 65001 > nul"); // 设置控制台输出编码为 UTF-8（Windows）

    // vector 构造与初始化
    vector<int> v1;                    // 空 vector
    vector<int> v2(5);                 // 5 个 0
    vector<int> v3(5, 1);              // 5 个 1
    vector<int> v4{1, 2, 3, 4, 5};     // 初始化列表
    int arr[] = {1, 2, 3};
    vector<int> v5(arr, arr + 3);      // 从数组构造
    vector<int> v6(v5.begin(), v5.end()); // 复制构造

    // 打印所有 vector
    printVector(v1);
    printVector(v2);
    printVector(v3);
    printVector(v4);
    printVector(v5);
    printVector(v6);

    // 访问 vector 元素
    int x = v6[1000];  // 访问第2个元素（无边界检查）
    int y = v6.at(1); // 访问第2个元素（有边界检查）
    int first = v6.front();  // 第一个元素
    int last = v6.back();    // 最后一个元素

    print(x);
    print(y);
    print(first);
    print(last);

    // vector 属性
    int size = v6.size();      // 当前元素数量
    int cap = v6.capacity();   // 当前分配的内存容量
    bool empty = v6.empty();   // 判断是否为空

    print(size);
    print(cap);
    print(empty);

    // vector 操作
    v6.reserve(100);  // 预分配100个元素的内存
    print(v6.capacity()); // 打印容量
    v6.shrink_to_fit();  // 减少容量以匹配大小（C++11）
    print(v6.capacity()); // 打印容量

    // vector 插入与删除
    v6.push_back(4);  // 末尾插入4
    printVector(v6);
    v6.insert(v6.begin() + 1, 99);  // 在第2个位置插入99
    printVector(v6);
    v6.insert(v6.end(), {100, 101, 102});  // 在末尾插入多个元素
    printVector(v6);
    v6.erase(v6.begin() + 1);  // 删除第2个元素
    printVector(v6);
    v6.pop_back();  // 删除最后一个元素
    printVector(v6);
    v6.clear();     // 清空所有元素
    printVector(v6);

    v6.assign(3, 7);  // 重置为3个7
    printVector(v6);
    v6.assign(arr, arr + 3);  // 重置为数组前3个元素2个元素
    printVector(v6);

    // 正向迭代遍历 vector
    for(auto it = v6.begin(); it != v6.end(); ++it) {
        print(*it);
    }

    // 反向迭代遍历 vector
    for(auto it = v6.rbegin(); it != v6.rend(); ++it) {
        print(*it);
    }

    // 二维 vector
    vector<vector<int>> vv{vector<int>(3, 0),vector<int>(3, 1), vector<int>(3, 2)}; // 二维 vector
    printVector(vv[0]);
    printVector(vv[1]);
    printVector(vv[2]);
    print(vv[0][0]);
    print(vv[1][1]);
    print(vv[2][2]);
    
    // 数组操作
    vector<int> v7{11, 2, 3, 14, 15, 6, 7, 8, 9, 10, 1, 12, 13, 4, 5};
    auto it = find(v7.begin(), v7.end(), 100) - v7.begin(); // 查找元素5
    print(it);
    sort(v7.begin(), v7.end()); // 排序
    printVector(v7);
    bool it1 = binary_search(v7.begin(), v7.end(), 5); // 二分查找元素5
    print(it1);

    // lambda 表达式，筛选大于5的元素，返回迭代器
    auto big5 = find_if(v7.begin(), v7.end(), [](int n) { return n > 5; }); // 查找元素5
    for(auto it = big5; it != v7.end(); ++it) {
        print(*it);
    }

    // copy_if 复制大于9的元素到新 vector
    vector<int> result;
    copy_if(v7.begin(), v7.end(), back_inserter(result), [](int n) { return n > 9; }); // 复制大于9的元素
    printVector(result);

    return 0;
}