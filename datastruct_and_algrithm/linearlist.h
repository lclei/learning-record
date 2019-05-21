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
