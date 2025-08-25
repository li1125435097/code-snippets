#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <regex>
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

    // string类型定义
    string s1;
    string s2 = string("hello world");
    string s3("welcome");
    string s4(5, 'a');
    string s5(s2.begin(),s2.begin()+5);
    
    print(s1);
    print(s2);
    print(s3);
    print(s4);
    print(s5);

    // string类型操作
    s1 = "abcdefg";
    print(s1);
    s1.assign("1234567890",4);
    print(s1);

    print("数量：" + to_string(s1.length()));
    print("size: " + to_string(s1.size()));
    print("容量：" + to_string(s1.capacity()));
    print("是否为空：" + to_string(s1.empty()));

    s1.reserve(64);
    print("容量：" + to_string(s1.capacity()));
    print(s1);
    s1.resize(32,'b');
    print("容量：" + to_string(s1.capacity()));
    print(s1);

    print("首字符：" + string(1,s1.front()));
    print("尾字符：" + string(1,s1.back()));

    print("访问字符：" + string(1,s1[6]));
    print("访问字符：" + string(1,s1.at(3)));

    // 插入字符串
    s1.append("love");
    print(s1);
    // 指定位置插入字符串
    s1.insert(6,"cpp");
    print(s1);
    // 插入字符串
    s1+="java";
    print(s1);
    // 插入字符
    s1.push_back('!');
    print(s1);
    // 删除字符串，从指定位置开始删除指定长度的字符
    s1.erase(10,20);
    print(s1);

    // 替换字符串,从指定位置开始替换指定长度的字符
    s1.replace(6,2,"python");
    print(s1);

    // 子串
    print(s1.substr(6,5));
    
    // 查找字符串
    print("查找字符串：" + to_string(s1.find("python")));
    print("查找字符串：" + to_string(s1.rfind("python")));
    print("查找字符串：" + to_string(s1.find_first_of("python")));
    print("查找字符串：" + to_string(s1.find_last_of("python")));

    // 字符串比较
    print("字符串比较：" + to_string(s1.compare("python")));
    print("字符串比较：" + to_string(s1.compare(0,5,"python")));
    print(s1.c_str());
    printIter(s1);

    print("请输入字符，按回车键结束：");
    string ss;
    // getline(cin,ss);
    print("你输入的字符是："+ss);

    regex ptn("(love)|(python)");
    smatch result;

    // 全匹配
    bool allMatch = regex_match(s1,ptn);
    print(allMatch);

    // 匹配一个
    bool partMatch = regex_search(s1,result,ptn);
    print(partMatch);
    printIter(result);

    // 匹配所有数据
    auto multiMatch = sregex_iterator(s1.begin(),s1.end(),ptn);
    // printIter(multiMatch);
    for(auto it = multiMatch; it != sregex_iterator();++it){
        print(it->str());
    }

    // 正则替换
    ss = regex_replace(s1,ptn,"wangbadan");
    print(ss);
    // 清空字符串
    s1.clear();



    return 0;
}