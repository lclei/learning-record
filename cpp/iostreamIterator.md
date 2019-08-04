# 输入输出流迭代器

## istream_iterator
- 创建一个istream_iterator，然后把它绑定到一个流
- 如果默认初始化，则是一个尾后值的迭代器
## ostream_iterator
```
ostream_iterator<T> out(os)  //out将类型为T的值写到os流中
ostream_iterator<T> out(os, d)  //out将类型为T的值写到os流中，且在后面加上字符串d
```
有了输入输出流迭代器，我们可以把输入输出流当作容器来使用，在拷贝，初始化等等有和容器操作相同的形式，代码比较好看

例子
```cpp
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>

int main(int argc, char* argv[])
{
    istream_iterator<int> in_iter(cin);
    istream_iterator<int> int_eof;
    vector<int> vec(in_iter,int_eof);   //[1]
    ostream_iterator<int> out_iter(cout," ");
    copy(vec.cbegin(),vec.cend(),out_iter); //[2]
}
```
该段代码接受屏幕输入，然后输出到屏幕，然后[1]和[2]都是容器使用迭代器的操作。比我们直接写循环显得简洁。
