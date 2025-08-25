#include <iostream>
#include <list>
#include <vector>
#include <deque>
#include <set>
#include <queue>
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

// 队列，默认为deque实现，默认为先进先出，头出尾进
int main() {
    system("chcp 65001 > nul"); // 设置控制台输出编码为 UTF-8（Windows）

    // 默认为大顶堆
    priority_queue<int> s1; 
    // 初始化小顶堆
    priority_queue<int, vector<int>, greater<int>> s2;
    priority_queue<string,deque<string>> s3;


    s1.push(1);
    s1.push(2);
    s2.push(3);
    s2.push(4);
    s3.push("hello");
    s3.push("world");

    s1.emplace(5);

    while (!s1.empty()) {
        print(s1.top());
        s1.pop();
    }
    
    print(s1.size());
    print(s2.size());
    print(s3.size());

    return 0;
}