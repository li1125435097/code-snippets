#include <iostream>
#include <vector>
using namespace std;
int main(int argc, char const *argv[])
{
    short a = 1;    // 2
    int b = 2;    // 4
    long c = 3;    // 4
    long long d = 4;    // 8
    unsigned short e = 5;  // 2
    unsigned int f = 6;    // 4
    unsigned long g = 7;    // 4
    unsigned long long h = 8;  // 8
    float i = 9;    // 4
    double j = 10;  // 8
    long double k = 11; // 8
    bool l = true;  // 1

    char m = 'a';    // 1
    char* n = "aaa";    // 8
    void* o = nullptr;    // 8

    cout << "数字: " << endl;
    cout << sizeof(a) << endl;
    cout << sizeof(b) << endl;
    cout << sizeof(c) << endl;
    cout << sizeof(d) << endl;
    cout << sizeof(e) << endl;
    cout << sizeof(f) << endl;
    cout << sizeof(g) << endl;
    cout << sizeof(h) << endl;
    cout << sizeof(i) << endl;
    cout << sizeof(j) << endl;
    cout << sizeof(k) << endl;
    cout << sizeof(l) << endl;

    cout << "字符: " << endl;
    cout << sizeof(m) << endl;
    cout << sizeof(n) << endl;

    cout << "void*: " << endl;
    cout << sizeof(o) << endl;

    cout << "数据类型转换: " << endl;
    cout << (int)true << endl;
    cout << (int)false << endl;
    cout << (int)m << endl;
    cout << std::atoi("1234") << endl;
    cout << *reinterpret_cast<int*>(n) << endl;
    cout << (bool)1 << endl;
    cout << (bool)0 << endl;
    return 0;
}
