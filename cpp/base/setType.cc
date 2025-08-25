#include <iostream>
#include <list>
#include <vector>
#include <deque>
#include <set>
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

// 基于红黑树实现，有序不重复序列，底层是二叉搜索树，内存开销较大，查找、插入、删除操作时间复杂度都是O(logn)
int main() {
    system("chcp 65001 > nul"); // 设置控制台输出编码为 UTF-8（Windows）

    // 集合构造
    set<int> st1; // 定义一个空的容器
    set<int> st2({1, 2, 3, 4, 1,5});      // 5个

    // 迭代器构造
    int arr[] = {1, 2, 3, 4, 5};
    set<int> st3(arr, arr + 3); // 5个

    // 拷贝构造
    set<int> st4(st3); // 5个

    // 自定义排序规则，降序构造
    set<int, greater<int>> st5({11, 22, 33, 44, 55}); // 5个

    printIter(st1);
    printIter(st2);
    printIter(st3);
    printIter(st4);
    printIter(st5);

    // 集合操作
    auto r = st5.insert(4); // 插入元素
    printIter(st5);
    print(r.second); // 插入结果
    print(*r.first); // 插入值

    st5.emplace(10); // 插入元素
    printIter(st5);
    st5.insert(arr,arr+ 3); // 插入数组
    printIter(st5);
    
    st5.erase(4); // 删除元素
    printIter(st5);
    st5.erase(st5.find(10), st5.end()); // 删除迭代器指向的元素
    printIter(st5);
    // st5.clear(); // 清空集合

    auto it = st5.find(10); // 查找元素
    auto it1 = st5.count(10); // 查找元素个数
    auto it2 = st5.lower_bound(22); // 查找第一个大于等于的元素
    auto it3 = st5.upper_bound(22); // 查找第一个大于的元素

    print(it != st5.end()); // 查找结果
    print(it1);
    print(*it2);
    print(*it3);

    print(st5.empty()); // 判断集合是否为空
    print(st5.size()); // 返回集合大小

    // 自定义排序规则，按除以3的余数排序
    struct MyCompare{
        bool operator()(const int& a, const int& b) const {
            return a%3 < b%3;
        }
    };
    set<int, MyCompare> st6({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}); // 5个
    printIter(st6);

    // 集合转其他容器
    vector<int> vec(st5.begin(), st5.end()); // set转vector
    list<int> ls(st5.begin(), st5.end()); // set转list
    deque<int> dq(st5.begin(), st5.end()); // set转deque
    printIter(vec);
    printIter(ls);
    printIter(dq);

    // 检查是否有重复
    int arr1[] = {1, 2, 3, 4, 5, 1, 2, 3, 4, 5};
    set<int>(arr1, arr1 + size(arr1)).size() == size(arr1) ? print("无重复") : print("有重复");

    return 0;
}