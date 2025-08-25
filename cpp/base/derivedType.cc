#include <iostream>
#include <vector>
using namespace std;

typedef int (*addsum)(int, int);
int sum(int, int);

int main(int argc, char const *argv[])
{
    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int* p = arr;
    int& ref = arr[0];
    int* p1 = &arr[0];
    int** p2 = &p;
    float a = 1.05;
    float* p3 = &a;
    float& ref2 = a;
    int (*addsum)(int, int) = sum;
    

    cout << "length: " << endl;
    cout << sizeof(arr) << endl;
    cout << sizeof(p) << endl;
    cout << sizeof(ref) << endl;
    cout << sizeof(p1) << endl;
    cout << sizeof(p2) << endl;

    cout << "pointer value: " << endl;
    cout << arr << endl;
    cout << p << endl;
    cout << ref << endl;
    cout << p1 << endl;
    cout << p2 << endl;

    cout << "value: " << endl;
    cout << *arr << endl;
    cout << *p << endl;
    cout << ref << endl;
    cout << *p1 << endl;
    cout << *p2 << endl;
    cout << addsum(11, 2) << endl;

    cout << "datetype cast: " << endl;
    cout << (int)a << endl;
    return 0;
}

int sum(int a, int b) { 
    return a+b; 
}