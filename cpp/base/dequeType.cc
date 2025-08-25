#include <iostream>
#include <list>
#include <vector>
#include <deque>
#include <algorithm>
using namespace std;

// 函数模板声明与定义（通常模板定义不能分离，需一起写出）
template <typename T>
void printIter(T& vec) {
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

// 双端队列，分段连续数组（块状结构）,可随机访问，可快速插入删除，头尾插入效率高，用于栈和队列底层实现
int main() {
    system("chcp 65001 > nul"); // 设置控制台输出编码为 UTF-8（Windows）

    deque<int> deq1; // 定义一个空的容器
    deque<int> deq2(5);      // 5个0
    deque<int> deq3(5, 10);  // 5个10

    int arr[] = {1, 2, 3};
    deque<int> deq4(arr, arr + 3);  // 1,2,3
    deque<int> deq5{1, 2, 3};  // 1,2,3

    int first = deq5.front();  // 第一个元素
    int last = deq5.back();    // 最后一个元素
    printIter(deq1);
    printIter(deq2);
    printIter(deq3);
    printIter(deq4);
    printIter(deq5);
    print(first);
    print(last);

    // 遍历
    for(auto it = deq5.begin(); it != deq5.end(); ++it) {
        print(*it);
    }

    // 插入
    deq5.push_back(44); // 在尾部插入元素
    deq5.push_front(70); // 在头部插入元素
    deq5.insert(deq5.begin()+2, 100); // 在指定位置插入元素
    printIter(deq5);

    // 删除
    deq5.push_back(2);      // 在尾部插入元素
    deq5.push_back(3);      // 在尾部插入元素
    deq5.pop_back();       // 删除尾部
    deq5.pop_front();      // 删除头部
    printIter(deq5);
    deq5.erase(next(deq5.begin(),1));  // 删除指定位置
    printIter(deq5);

    // 其他操作
    int size = deq5.size();
    bool isEmpty = deq5.empty();
    print(size);
    print(isEmpty);

    // 修改大小
    deq5.resize(10);      // 扩容至10个元素（默认补0）
    deq5.resize(4);       // 截断至前2个元素
    printIter(deq5);

    auto it = deq5.max_size(); // 容器最大容量
    print(it);

    sort(deq5.begin(), deq5.end()); // 排序
    printIter(deq5);

    vector<int> vec(deq5.begin(), deq5.end()); // deque转vector
    list<int> deq(deq5.begin(), deq5.end()); // deque转list
    printIter(deq);

    return 0;
}