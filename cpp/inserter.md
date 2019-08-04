# 插入迭代器

接受一个容器，生成一个迭代器，实现向给定容器插入元素

## 操作
> it = t

在it指定的当前位置插入值t，且根据插入迭代器的类型不同，分别调用c.push_back(t), c.oush_front(t), c.insert(t,p)

> *it, it++, ++it

有这些形式，但是什么都不做

##类型

- back_inserter
创建一个使用push_back的迭代器

- front_inserter
创建一个使用push_front的迭代器

- inserter
创建一个使用insert的迭代器
若it是一个inserter则以下代码
```cpp
*it = val;
```
与
```
it = c.insert(it, val);
++it;
```
效果一样
示例:
```cpp
#include <iostream>
#include <list>
#include <iterator>
#include <algorithm>

using namespace std;

void Output(const list<int> &a){
    ostream_iterator<int> out_iter(cout," ");
    copy(a.cbegin(),a.cend(),out_iter);
    cout << endl;
}
int main(int argc, char *argv[])
{
    list<int>a1 = {1,2,3,4},a2,a3,a4;
    copy(a1.cbegin(),a1.cend(),inserter(a2,a2.begin()));
    copy(a1.cbegin(),a1.cend(),front_inserter(a3));
    copy(a1.cbegin(),a1.cend(),back_inserter(a4));
    Output(a2);
    Output(a3);
    Output(a4);
}
```
输出
> 1 2 3 4
4 3 2 1
1 2 3 4

