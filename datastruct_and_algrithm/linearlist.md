# 线性表

抽象数据类型LinearList{
  实例
    0或多个元素的有序集合
  操作
    Creat() : 创建一个空线性表
    Destroy() : 删除表
    IsEmpty() : 如果表为空返回true，否则返回false
    Length() : 返回表的大小
    Find(k,x) : 寻找表中第k个元素，并把它保存到x中；如果不存在，否则返回false
    Search(x) : 返回元素x在表中的位置；如果x不在表中，则返回0
    Delete(k,x): 删除表中第k个元素，并把它保存到x中；返回修改后的线性表
    Insert(k,x): 在第k个元素的后面插入x，返回修改后的线性表
    Output(out): 把线性表放入输出流out中
}

按照定义给出的LinearList class定义
```cpp
template<typename T>
class LinearList{
public:
  LinearList(int MaxListSize = 10); //构造函数
  ~LinearList(){delete [] element;}; //析构函数
  bool IsEmpty() const {return length == 0;}
  bool Find(int k, T& x) const; //末尾的const表示常量成员函数，修饰隐式的this指向常量类型
  int Length() const {return length;}
  int Search(const T& x) const;
  LinearList<T>& Delete(int k, T& x);//删除第k个元素并将它返回至x中
  LinearList<T>& Insert(int k, const T& x); //在第k个元素的后面插入x
  void Output(ostream& out) const;
private:
  int length;
  int MaxSize;
  T *element;
};
```
>知识点：成员函数末尾的const  
成员函数是有隐含的指向class本身的常量指针this，例```int Length() const {return length;}```实际上的意思应该有```nt Length(LinearList<T> * const this) {return this->length;}```，我们除了希望this本身是常量指针外，还希望this指向的对象也不可修改的话，即```const LinearList<T> * const this```，由于this是隐式的，cpp语法将第一个const放在成员函数的参数列表最后。

下面是具体实现
```cpp
// linearlist.h
// 定义linearlist类
// lcl 20190521

#ifndef _LINEARLIST_H
#define _LINEARLIST_H

#include <iostream>
#include "exception.h"
using namespace std;

template<typename T>
class LinearList{
public:
  LinearList(int MaxListSize = 10); //构造函数
  ~LinearList(){delete [] element;}; //析构函数
  bool IsEmpty() const {return length == 0;}
  bool Find(int k, T& x) const; //末尾的const表示常量成员函数，修饰隐式的this指向常量类型
  int Length() const {return length;}
  int Search(const T& x) const;
  LinearList<T>& Delete(int k, T& x);//删除第k个元素并将它返回至x中
  LinearList<T>& Insert(int k, const T& x); //在第k个元素的后面插入x
  void Output(ostream& out) const;
private:
  int length;
  int MaxSize;
  T *element;
};

template<typename T>
LinearList<T>::LinearList(int MaxListSize){
  //构造函数
  MaxSize = MaxListSize;
  element = new T[MaxSize];
  length = 0;
}
template<typename T>
bool LinearList<T>::Find(int k,T& x) const{
  //把第k个元素取到x中
  if(k<1 || k>length) return false;
  x = element[k-1];
  return true;
}
template<typename T>
int LinearList<T>::Search(const T& x) const{
  // 查找x，返回位置
  for(int i=0;i<length;i++){
    if(element[i] == x) return ++i;
  }
  return 0;
}
template<typename T>
LinearList<T>& LinearList<T>::Delete(int k,T &x){
  // 删除第k个元素并将它返回至x中
  if(Find(k,x)){
    for(int i=k;i<length;i++){
      element[i-1]=element[i];
    }
    length--;
    return *this;
  }
  else throw OutOfBounds();
}
template<typename T>
LinearList<T>& LinearList<T>::Insert(int k,const T &x)
{
  //在第k个元素的后面插入x
  // 如果不存在第k个元素，则引发异常OutOFBounds
  // 如果表已经满了，引发异常NoMem
  if(k<0||k>length) throw OutOfBounds();
  if(length==MaxSize) throw NoMem();
  for(int i=length-1;i>=k;i--){
    element[i]=element[i-1];
  }
  element[k]=x;
  length++;
  return *this;
}
template<typename T>
void LinearList<T>::Output(ostream& out) const{
  for(int i=0;i<length;i++){
    out << element[i]<<" ";
  }
}
template<typename T>
ostream& operator<<(ostream& out, const LinearList<T>& x){
  x.Output(out);
  return out;
}

#endif
```
```cpp
// exceetion.h
// 定义异常类
// lcl 20190521
#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#include <iostream>
using namespace std;

class NoMem{
public:
  NoMem(){}
};

void my_new_handler(){
  throw NoMem();
};

new_handler Old_Handler_=set_new_handler(my_new_handler);

class OutOfBounds{
public:
  OutOfBounds(){cout << "Out Of Bounds!" << endl;}
};

#endif
```
```cpp
// linearlistTest.cpp
// 测试linearlist类功能，main
// lcl 20190521

#include "linearlist.h"
#include "exception.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[]){
  try{
    LinearList<int> L(5);
    cout << "Length = " << L.Length() <<endl;
    cout << "IsEmpty = " << L.IsEmpty() <<endl;
    L.Insert(0,2).Insert(1,6);
    cout << "List is " << L <<endl;
    cout << "IsEmpty = " << L.IsEmpty() << endl;
    int z;
    L.Find(1,z);
    cout << "First element is " << z <<endl;
    cout << "Length = " <<L.Length() << endl;
    L.Delete(1,z);
    cout << "Delete element is " << z << endl;
    cout << "List is " << L << endl;
  }
  catch(...){
    cerr << "An Exception has occurred" << endl;
  }
}
```
最后展示运行结果
```
lcl@FX50JK:~/Documents/hub/learning-record/datastruct_and_algrithm$ g++ linearlistTest.cpp -g -o linearlistTest
lcl@FX50JK:~/Documents/hub/learning-record/datastruct_and_algrithm$ ./linearlistTest
Length = 0
IsEmpty = 1
List is 2 6
IsEmpty = 0
First element is 2
Length = 2
Delete element is 2
List is 6
```
