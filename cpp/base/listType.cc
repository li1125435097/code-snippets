#include <iostream>
#include <list>
#include <vector>
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

// 双端链表，每个节点双向指针，内存消耗比较大，适合频繁插入删除操作
int main() {
    system("chcp 65001 > nul"); // 设置控制台输出编码为 UTF-8（Windows）

    list<int> lst1; // 定义一个空的 list 容器
    list<int> lst2(5);      // 5个0
    list<int> lst3(5, 10);  // 5个10

    int arr[] = {1, 2, 3};
    list<int> lst4(arr, arr + 3);  // 1,2,3
    list<int> lst5{1, 2, 3};  // 1,2,3

    int first = lst5.front();  // 第一个元素
    int last = lst5.back();    // 最后一个元素
    printIter(lst1);
    printIter(lst2);
    printIter(lst3);
    printIter(lst4);
    printIter(lst5);
    print(first);
    print(last);

    // 遍历
    for(auto it = lst5.begin(); it != lst5.end(); ++it) {
        print(*it);
    }

    // 插入
    lst5.push_back(44); // 在尾部插入元素
    lst5.push_front(70); // 在头部插入元素
    lst5.insert(next(lst5.begin(),2), 100); // 在指定位置插入元素
    printIter(lst5);

    // 删除
    lst5.push_back(2);      // 在尾部插入元素
    lst5.push_back(3);      // 在尾部插入元素
    lst5.pop_back();       // 删除尾部
    lst5.pop_front();      // 删除头部
    printIter(lst5);
    lst5.erase(next(lst5.begin(),1));  // 删除指定位置
    lst5.remove(2);        // 删除所有值为2的元素
    printIter(lst5);

    // 其他操作
    int size = lst5.size();
    bool isEmpty = lst5.empty();
    print(size);
    print(isEmpty);

    // 修改大小
    lst5.resize(10);      // 扩容至10个元素（默认补0）
    lst5.resize(2);       // 截断至前2个元素
    printIter(lst5);

    lst5.sort();
    printIter(lst5);

    lst5.reverse();
    printIter(lst5);

    lst5.merge(lst4);  // 合并
    printIter(lst5);

    
    vector<int> vec(lst5.begin(), lst5.end()); // list转vector
    printIter(vec);

    return 0;
}