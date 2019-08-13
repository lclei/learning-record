# 智能指针

> \#include \<memory>

三种智能指针
## shared_ptr

有引用计数，当最后一个shared_ptr被销毁，其指向的对象将被析构。

例子：
```cpp
#include <iostream>
#include <memory>
#include <iterator>

using namespace std;

class Foo{
    public:
    Foo() {p = new int[10];cout << "new@" << p <<endl;}
    ~Foo() {cout << "release@" << p << endl;delete [] p;p=nullptr;}
    int *p;
};

void smartptr()
{
    shared_ptr<Foo> sp1 = make_shared<Foo>();
    auto sp2 = make_shared<Foo>();
}
void ptr()
{
    auto p = new Foo;
}
int main(int argc,char* argv[])
{
    smartptr();
    ptr();
    return 0;
}
```
其输出结果为
> new@0x1001370
new@0x10013b8
release@0x10013b8
release@0x1001370
new@0x1001368

make_shared\<>是库函数，将会new一个对象，然后绑定到shared_ptr

- 在子函数smartptr中两个临时变量是共享指针，在退出子函数时会被销毁，销毁时调用了对象的析构函数。
- 而ptr函数里只是普通指针，退出销毁时没有调用析构函数，造成内存泄漏。

删除器

使用智能指针还可以定义自己的释放操作，这是针对那些没有合适析构函数的对象或是c语言的代码
```cpp
#include <memory>
#include <iostream>
using namespace std;

typedef int fd;

fd fileid = 0;

fd open()
{
    cout << "open " << fileid << endl;
    return fileid++;
}
void close(fd f)
{
    cout << "close " << f << endl;
    fileid--;
}

void end_fd(fd* f){close(*f);}

void foo()
{
    fd g = open();
    shared_ptr<fd> sp(&g,end_fd);
}

int main(int argc, char *argv[])
{
    foo();
}
```
输出
>open 0
close 0

需要注意的是传入的删除器函数的参数必须是智能指针保存的指针类型才可以通过编译。

## unique_ptr

不能拷贝，不能赋值，拥有指向的对象

可以直接初始化
```cpp
unique_ptr<string> p1(new string("I am unique"))
```
```cpp
unique_ptr<string> p2(p1); //错误，不支持拷贝
```
```cpp
unique_ptr<string> p2;
p2=p1;//错误，不支持赋值
```

需要转移控制权的话，可以用
```cpp
unique_ptr<string> p2(p1.release())
```
或者
```cpp
unique_ptr<string> p2;
p2.reset(p1.release());
```

# weak_ptr

需要一个shared_ptr来初始化它，不影响shared_ptr的计数
```cpp
weak_ptr<T> wp(sp)
```
操作
```cpp
wp.reset()  //置为空
wp.use_count()  //返回指向对象的shared_ptr的数量
wp.expired() //wp.use_count()为0，返回true
wp.lock()  //wp.expired()为true，则返回一个共享的shared_ptr，否则返回一个空的shared_ptr
```
