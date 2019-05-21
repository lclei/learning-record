# 多态 polymorphism

- 一个接口，多种方法
- 使用虚函数实现

基类成员函数添加了virtual关键字，该函数在执行时绑定。否则在编译时绑定。所以

**基类希望其派生类进行覆盖的函数，定义为虚函数**


例子
```c
#include <iostream>
using namespace std;

class Animal{
public:
  void sound(){
    cout << "??" << endl;
  }
};



class Cat : public Animal{
public:
  void sound(){
    cout << "Miao Miao" << endl;
  }
};

class Dog : public Animal{
public:
  void sound(){
    cout << "Wang Wang" << endl;
  }
};

int main(int argc, char **argv){
  Animal *animal;
  animal = new Cat;
  animal->sound();
  animal = new Dog;
  animal->sound();
}
```
输出
>lcl@FX50JK:~/Documents/hub/learning-record/cpp$ ./polymorphism  
??  
??

可见animal指针虽然指向子类Cat和Dog，但是并没用他们的sound方法。基类使用virtual关键字

```c
class Animal{
public:
  virtual void sound(){
    cout << "??" << endl;
  }
};
```
输出
>lcl@FX50JK:~/Documents/hub/learning-record/cpp$ ./polymorphism   
Miao Miao  
Wang Wang
