# 右值引用

- 使用&&表示
- 只能绑定到一个将要销毁的对象
- 左值持久，右值短暂
- 将左值转换为对应的右值使用std::move

```cpp
int i = 42;
int &r = i;     //左值引用，ok
int &&rr = i;   //错误，不能把右值引用绑定到左值上
int &r2 = i*42; //错误，i*42是一个右值
const int &r3 = i*42;   //ok，const引用可以绑定到右值
int &&rr2 = i*42;   //ok，右值引用绑定右值
```
## 移动构造和移动赋值

例如这样一个类StrVec中有移动构造和移动赋值，使用&&作为参数

```cpp
// StrVec class, a class like vector but only store string as element
// CppPrimer 13.5
// lcl 20190712

#include <iostream>
#include <string>
#include <memory>
using namespace std;

class StrVec {
    public:
    StrVec(): elements(nullptr), first_free(nullptr), cap(nullptr){}
    StrVec(const StrVec &); // copy constructor
    StrVec(StrVec &&) noexcept; // move constructor
    StrVec &operator=(const StrVec&); // copy assignment
    StrVec &operator=(StrVec&&) noexcept; //move assignment
    ~StrVec();
    void push_back(const std::string&);
    size_t size() const {return first_free - elements;}
    size_t capacity() const {return cap - elements;}
    std::string *begin() const { return elements;}
    std::string *end() const { return first_free;}
    private:
    std::allocator<std::string> alloc;
    void chk_n_alloc()
    {
        if(size()==capacity()) 
        reallocate();
    }
    std::pair<std::string*, std::string*> alloc_n_copy
        (const std::string*, const std::string*);
    void free();
    void reallocate();
    std::string *elements;   //point to first element
    std::string *first_free;    //point to first free element
    std::string *cap;   //point to rear;
};

void StrVec::push_back(const string& s)
{
    chk_n_alloc();
    alloc.construct(first_free++, s);
}

pair<string*, string*>
StrVec::alloc_n_copy(const string *b, const string* e)
{
    auto data = alloc.allocate(e-b);
    return {data, uninitialized_copy(b,e,data)};
}

void StrVec::free()
{
    if(elements){
        for(auto p = first_free; p!=elements;alloc.destroy(--p));
        alloc.deallocate(elements, cap-elements);
    }
}

StrVec::StrVec(const StrVec&s)
{
    auto newdata = alloc_n_copy(s.begin(), s.end());
    elements = newdata.first;
    first_free = cap = newdata.second;
}

StrVec::~StrVec()
{
    free();
}

StrVec& StrVec::operator=(const StrVec &rhs)
{
    // 先分配大小相同的内存拷贝，再释放掉原data
    auto data = alloc_n_copy(rhs.begin(), rhs.end());
    free();
    elements = data.first;
    first_free = cap = data.second;
    return *this;
}

void StrVec::reallocate()
{
    auto newcapacity = size() ? 2 * size() : 1;
    auto newdata = alloc.allocate(newcapacity);
    auto dest = newdata;
    auto elem = elements;
    for( size_t i = 0; i!=size();++i)
        // 使用了std::move，从而会调用移动构造函数
        alloc.construct(dest++,std::move(*elem++));
    free();

    elements = newdata;
    first_free = dest;
    cap = elements + newcapacity;
}

StrVec::StrVec(StrVec &&s) noexcept //移动操作不抛出任何异常
    :elements(s.elements), first_free(s.first_free), cap(s.cap)
{
    // 令s进入这样的状态，对其进行析构是安全的？
    // 这里令元素为空，析构函数将不能释放空间
    // 因为元素已经转给移动后的对象，空间不应该释放，所以令他们为空
    s.elements = s.first_free = s.cap = nullptr;
}

// 使用noexcept告知编译器不会抛出异常

StrVec& StrVec::operator=(StrVec &&rhs) noexcept
{
    // 处理自赋值的情况
    if(this != &rhs){
        free();
        elements = rhs.elements;
        first_free = rhs.first_free;
        cap = rhs.cap;
        rhs.elements = rhs.first_free = rhs.cap = nullptr;
    }
}

// test
int main(int argc, char* argv[])
{
    StrVec sv,sv1;
    std::cout << sv.size() << " "
              << sv.capacity() << std::endl;
    sv.push_back("first line");
    sv.push_back("second_line");
    sv1 = sv;
    std::cout << sv1.size() << " "
              << sv1.capacity() << std::endl;
}
```