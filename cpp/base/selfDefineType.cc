
#include <iostream>
#include <string>

// 动物基类
class Animal {
protected:
    std::string name;
    int age;

public:
    Animal(const std::string& name, int age) : name(name), age(age) {}

    virtual void makeSound() const = 0;  // 纯虚函数
    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", Age: " << age << " years" << std::endl;
    }

    virtual ~Animal() {}  // 虚析构函数
};

// 狗类
class Dog : public Animal {
private:
    std::string breed;

public:
    Dog(const std::string& name, int age, const std::string& breed)
        : Animal(name, age), breed(breed) {}

    void makeSound() const override {
        std::cout << "Woof! Woof!" << std::endl;
    }

    void displayInfo() const override {
        Animal::displayInfo();
        std::cout << "Breed: " << breed << std::endl;
    }
};

// 猫类
class Cat : public Animal {
private:
    std::string color;
    int weight;

public:
    Cat(const std::string& name, int age, const std::string& color)
        : Animal(name, age), color(color),weight(128) {}

    void makeSound() const override {
        std::cout << "Meow! Meow!" << std::endl;
    }

    void displayInfo() const override {
        Animal::displayInfo();
        std::cout << "Color: " << color << std::endl;
    }

    void run() const;
};

void Cat::run() const{
    std::cout << "Cat::run()" << std::endl;
}

struct Reactangle {
    double x,y;
    Reactangle(double x, double y):x(x),y(y) {}
    double area() const {return x*y;}
};

/*
 * 主函数：演示多态和类型转换
 * 创建Dog和Cat对象，通过基类指针调用它们的方法
 */
int main() {
    // 创建Dog对象，名称为"Buddy"，年龄为3，品种为"Golden Retriever"
    Dog myDog("Buddy", 3, "Golden Retriever");
    // 创建Cat对象，名称为"Whiskers"，年龄为2，颜色为"Black"
    Cat myCat("Whiskers", 2, "Black");

    // 创建基类指针数组，分别指向Dog和Cat对象
    Animal* animals[] = {&myDog, &myCat};

    // 遍历动物数组，通过基类指针调用子类的方法
    for (auto animal : animals) {
        // 调用displayInfo()方法，显示动物信息
        animal->displayInfo();
        // 调用makeSound()方法，发出动物声音
        animal->makeSound();
        // 强制指针类型，访问子类的run函数，如果子类没有该函数，会填入垃圾数据
        static_cast<Cat*>(animal)->run();
        std::cout << "----------------" << std::endl;
    }

    Reactangle r(3,4);
    std::cout << r.area() << std::endl;

    using std::cout;
    using std::endl;
    cout << "使用命名空间" << endl;

    using iPtr = int*;
    int a = 99;
    iPtr p = &a;
    cout << *p << endl;
    return 0;
}
