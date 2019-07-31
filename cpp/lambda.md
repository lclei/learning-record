# lambda表达式

## 介绍
一个lambda表达式表示一个可调用的代码单元，可以将其理解为一个未命名的内联函数。形式为
```
[capture list](parameter list) -> return type { function body}
//必须使用尾置返回
```
几个特点
- 必须使用尾置返回
- 在创建时捕获
- 只有一行return语句时可省略返回类型，否则默认为void
- 一般在给算法传递谓词时可以使用

## lambda表达式的捕获
- 值捕获
```cpp
void fun1()
{
    size_t v1 = 21;
    auto f = [v1]{return v1;}; //省略了参数列表
    v1 = 0;
    cout << f();
}
```
输出21
- 引用捕获
```cpp
void fun2()
{
    size_t v2 = 21;
    auto f2 = [&v2] {return v2;};
    v2 = 0;
    cout << f2();
}
```
将输出0
- 隐式捕获
只在捕获列表里加=（值）或者&（引用），由编译器来推断
```cpp
void fun3()
{
    size_t v3 = 21;
    auto f3 = [&] {return v3;};
    v3 = 0;
    cout << f3();
}
```

输出0
- 混合捕获
即在捕获列表开始使用=或者&隐式捕获，再对不同的捕获变量使用显式捕获。此显式捕获必须与开始的隐式捕获不同（值或引用）

## mutable
对于值捕获，lambda表达式不能改变捕获的参数，如
```cpp
void fun4()
{
    size_t v4 =21;
    auto f4 = [=] (int i)  {return (++v4) + i;};
    cout << endl << f4(9);
}
```
编译错误
> error: increment of read-only variable 'v4'|

可以在参数列表后面加mutable，同时即使没有参数，参数列表也不能省略了。
```cpp
void fun4()
{
    size_t v4 =21;
    auto f4 = [=] (int i) mutable {return (++v4) + i;};
    cout << endl << f4(9);
}
```
而用引用捕获时就不需要了
```cpp
void fun4()
{
    size_t v4 =21;
    auto f4 = [&] (int i)  {return (++v4) + i;};
    cout << endl << f4(9);
}
```

## 返回
当函数体不是只有一条return语句时需要指定返回类型，且必须用尾置返回类型。