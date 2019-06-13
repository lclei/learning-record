# converting constructor

当类的构造函数只有一个实参，就相当于有了一个隐式类型转换的方法
```cpp
// lcl 20190611
// tmp.cpp
// show implicit converting caused by converting constructor
#include <iostream>
#include <string>
using namespace std;

class foo{
public:
  foo(string s): str(s) {}
  void add(const foo &f){str += f.str;}
  void print(){cout << str;}
private:
  string str;
};

int main(){
  string s = "something";
  foo f("here");
  f.add(s);
  f.print();
}
```
上例中foo类的f使用add成员函数，参数本应是另一个foo类，但是这里传入string对象仍然可以，就是因为有**隐式的类型转换**，而隐式的类型转换的来源在于有一个**单参数的构造函数**。

一个要注意的地方是**类类型转换只允许一步转换**

- ```f.add("sth")```错误，因为需要字符串常量转成string，string再转foo，共两步。
- ```f.add(string("sth"))```正确
- ```f.add(foo("sth"))```正确

如果不希望发生这种隐式类型转换，需要在相关构造函数前加关键字 *explicit*，而且explicit关键字只能在类内定义或声明前加，不应在类外定义时重复。

```cpp
class foo{
public:
  explicit foo(string s): str(s) {}
  void add(const foo &f){str += f.str;}
  void print(){cout << str;}
private:
  string str;
};
```
此时已经不会发生隐式转换，比如```f.add(string("sth"))```。然而仍可以做到显式强制转换
- ```f.add(foo("sth"))```正确
- ```f.add(static_cast<foo>("sth"))``` 正确
