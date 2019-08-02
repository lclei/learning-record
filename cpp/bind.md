# bind

> auto g = bind(f, arg_list)

用我自己的话来说，就是把原来函数f的参数重新安排，可以直接绑定值，也可以使用占位符placeholder，达到减少参数或者调换参数顺序的作用。

例如
```cpp
#include <iostream>
#include <string>
#include <functional>

using namespace std;
using namespace std::placeholders; // for _1, _2 ...

string f(string a1, string a2, string a3, string a4)
{
    return a1+a2+a3+a4;
}

int main(int argc, char *argv[])
{
    auto g  = bind(f,_1,"Two","Three","Four");
    cout << g("One") << endl;
    return 0;
}
```
输出
> OneTwoThreeFour

这样就把一个四个参数的函数f(4)转成了一个参数的函数g(1)，其中的占位符_1表示g的第一个参数。


如果交换参数的顺序，也是使用占位符，比如上例
```cpp
int main(int argc, char *argv[])
{
    auto g  = bind(f,_4,_2,_3,_1);
    cout << g("One","Two","Three","Four") << endl;
    return 0;
}
```
输出
> FourTwoThreeOne

所以占位符就是表示g中的第几个参数，再按顺序传给f。

可以用于调整参数顺序，绑定部分参数等，做一个函数适配器的功能。
