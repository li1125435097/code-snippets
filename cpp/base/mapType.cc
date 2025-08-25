#include <iostream>
#include <list>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <algorithm>
using namespace std;

// 函数模板声明与定义（通常模板定义不能分离，需一起写出）
template <typename T>
void printIter(T& map) {
    cout << "[ ";
    for (const auto& item : map) {
        cout << item.first << "->" << item.second << " ";
    }
    cout << "]" << endl;
}

template <typename T>
void print(const T& t) {
    cout << t << endl;
}

// 基于红黑树实现，有序不重复序列，底层是二叉搜索树，内存开销较大，查找、插入、删除操作时间复杂度都是O(logn)
int main() {
    system("chcp 65001 > nul"); // 设置控制台输出编码为 UTF-8（Windows）

    // 集合构造
    map<int,int> st1; // 定义一个空的容器
    map<int, int> st2({{11,1}, {22,2}, {33,3}, {44,4}}); // 4个

    // 迭代器构造
    pair<int,int> arr[]= {{1, 2}, {3, 4}, {5, 6}};
    map<int,int> st3(arr, arr + 3); // 3个

    // 拷贝构造
    map<int,int> st4(st2); // 3个

    // 自定义排序规则，降序构造

    printIter(st1);
    printIter(st2);
    printIter(st3);
    printIter(st4);

    // 集合操作
    auto r = st4.insert({5,7}); // 插入元素
    printIter(st4);
    print(r.second); // 插入结果

    st4[15] = 8; // 插入元素
    st4.emplace(10,1); // 插入元素
    printIter(st4);
    st4.insert(arr,arr+ 3); // 插入数组
    printIter(st4);
    
    st4.erase(1); // 删除元素
    printIter(st4);
    st4.erase(st4.begin(), st4.find(10)); // 删除迭代器指向的元素
    printIter(st4);
    // st4.clear(); // 清空集合

    auto it = st4.find(10); // 查找元素
    auto it1 = st4.count(10); // 查找元素个数
    auto it2 = st4.lower_bound(22); // 查找第一个大于等于的元素
    auto it3 = st4.upper_bound(22); // 查找第一个大于的元素


    print(it != st4.end()); // 查找结果
    print(it1);
    print((*it2).first);
    print((*it3).first);

    print(st4.empty()); // 判断集合是否为空
    print(st4.size()); // 返回集合大小
    print(st4.max_size()); // 返回集合最大容量
    // auto [key,val] = *st4.begin(); // 返回集合第一个元素
    // print(key); // 返回集合第一个元素
    // print(val); // 返回集合第一个元素
    int key,val;
    tie(key,val) = *st4.begin(); // 返回集合第一个元素
    print(key); // 返回集合第一个元素
    print(val); // 返回集合第一个元素


    // 集合转其他容器
    vector<pair<int,int>> vec(st4.begin(), st4.end()); // set转vector
    list<pair<int,int>> ls(st4.begin(), st4.end()); // set转list
    deque<pair<int,int>> dq(st4.begin(), st4.end()); // set转deque
    set<pair<int,int>> st5(vec.begin(), vec.end()); // vector转set
    printIter(vec);
    printIter(ls);
    printIter(dq);
    printIter(st5);

    return 0;
}