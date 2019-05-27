# 线性表

抽象数据类型ADT  

LinearList{  
  实例

    0或多个元素的有序集合

  操作

    empty(): 若表空，则返回true,否则返回fasle
    size(): 返回线性表的大小
    get(index): 返回线性表里索引为index的元素
    indexOf(x): 返回线性表中第一次出现x的索引，若x不存在，则返回-1
    erase(index): 删除索引为index的元素
    insert(index,x): 把x插入到index索引处
    output():从左到右输出表元素
  }

按照定义给出的LinearList class定义，可以使用抽象类来表示。一个抽象类包含着没有实现代码的成员函数，称为纯虚函数（pure virtual function)，纯虚函数使用数字0作为初始值来说明，如
```cpp
virtual int  myPureVirtualFunction(int x)=0;
```
抽象类定义如下：
```cpp
template<class T>
class LinearList
{
public:
  virtual ~linearList() {};
  virtual bool empty() const = 0;
  virtual int size() const = 0;
  virtual T& get(int theIndex) const = 0;
  virtual int indexOf(const T& theElement) const = 0;
  virtual void erase(int theIndex) = 0;
  virtual void insert(int theIndex, const T& theElement) = 0;
  virtual void output(ostream& out) const = 0;
}
```
>知识点：成员函数末尾的const  
成员函数是有隐含的指向class本身的常量指针this，例```int Length() const {return length;}```实际上的意思应该有```nt Length(LinearList<T> * const this) {return this->length;}```，我们除了希望this本身是常量指针外，还希望this指向的对象也不可修改的话，即```const LinearList<T> * const this```，由于this是隐式的，cpp语法将第一个const放在成员函数的参数列表最后。

下面是list的数组实现
```cpp
// array implementation of a linear list
// derives from abstract class linearList just to make sure
// all methods of the ADT are implemented
// USES STL ALGORITHMS TO SIMPLIFY CODE

#ifndef arrayList_
#define arrayList_

#include<iostream>
#include<sstream>
#include<string>
#include<algorithm>
#include<iterator>
#include "linearList.h"
#include "myExceptions.h"
#include "changeLength1D.h"

using namespace std;

template<class T>
class arrayList : public linearList<T>
{
   public:
      // constructor, copy constructor and destructor
      arrayList(int initialCapacity = 10);
      arrayList(const arrayList<T>&);
      ~arrayList() {delete [] element;}

      // ADT methods
      bool empty() const {return listSize == 0;}
      int size() const {return listSize;}
      T& get(int theIndex) const;
      int indexOf(const T& theElement) const;
      void erase(int theIndex);
      void insert(int theIndex, const T& theElement);
      void output(ostream& out) const;

      // additional method
      int capacity() const {return arrayLength;}

   protected: //[1]
      void checkIndex(int theIndex) const;
            // throw illegalIndex if theIndex invalid
      T* element;            // 1D array to hold list elements
      int arrayLength;       // capacity of the 1D array
      int listSize;          // number of elements in list
};

template<class T>
arrayList<T>::arrayList(int initialCapacity)
{// Constructor.
   if (initialCapacity < 1)
   {ostringstream s;
    s << "Initial capacity = " << initialCapacity << " Must be > 0";
    throw illegalParameterValue(s.str());
   }
   arrayLength = initialCapacity;
   element = new T[arrayLength];
   listSize = 0;
}

template<class T>
arrayList<T>::arrayList(const arrayList<T>& theList)
{// Copy constructor.
   arrayLength = theList.arrayLength;
   listSize = theList.listSize;
   element = new T[arrayLength];
   copy(theList.element, theList.element + listSize, element);
}

template<class T>
void arrayList<T>::checkIndex(int theIndex) const
{// Verify that theIndex is between 0 and listSize - 1.
   if (theIndex < 0 || theIndex >= listSize)
   {ostringstream s;
    s << "index = " << theIndex << " size = " << listSize;
    throw illegalIndex(s.str());
   }

}

template<class T>
T& arrayList<T>::get(int theIndex) const
{// Return element whose index is theIndex.
 // Throw illegalIndex exception if no such element.
   checkIndex(theIndex);
   return element[theIndex];
}

template<class T>
int arrayList<T>::indexOf(const T& theElement) const
{// Return index of first occurrence of theElement.
 // Return -1 if theElement not in list.

   // search for theElement
   int theIndex = (int) (find(element, element + listSize, theElement)
                         - element);

   // check if theElement was found
   if (theIndex == listSize)
     // not found
     return -1;
   else return theIndex;
 }

template<class T>
void arrayList<T>::erase(int theIndex)
{// Delete the element whose index is theIndex.
 // Throw illegalIndex exception if no such element.
   checkIndex(theIndex);

   // valid index, shift elements with higher index
   copy(element + theIndex + 1, element + listSize,
                                element + theIndex);

   element[--listSize].~T(); // invoke destructor
}

template<class T>
void arrayList<T>::insert(int theIndex, const T& theElement)
{// Insert theElement so that its index is theIndex.
   if (theIndex < 0 || theIndex > listSize)
   {// invalid index
      ostringstream s;
      s << "index = " << theIndex << " size = " << listSize;
      throw illegalIndex(s.str());
   }

   // valid index, make sure we have space
   if (listSize == arrayLength)
      {// no space, double capacity
         changeLength1D(element, arrayLength, 2 * arrayLength);
         arrayLength *= 2;
      }

   // shift elements right one position
   copy_backward(element + theIndex, element + listSize,
                 element + listSize + 1);

   element[theIndex] = theElement;

   listSize++;
}

template<class T>
void arrayList<T>::output(ostream& out) const
{// Put the list into the stream out.
   copy(element, element + listSize, ostream_iterator<T>(cout, "  ")); //[2]
}

// overload <<
template <class T>
ostream& operator<<(ostream& out, const arrayList<T>& x)
   {x.output(out); return out;}

#endif

```
[1] protected：派生类可以访问继承的protected，public成员，不能访问private成员

[2] ostream_iterator需要包含头文件<iterator>，```ostream_iterator<string> os( cout, " " );```上述第二个参数可以是C-Sty1e字符串，也可以是字符串常量。它用来表示各个元素被输出时的分隔符，默认情形下输出的各个元素并无任何分隔符。

测试结果

```cpp

// test the class arrayList that uses STL algorithms

#include<iostream>
#include "linearlist.h"
using namespace std;
int main()
{
   // test constructor
   linearList<double> *x = new arrayList<double>(20);
   arrayList<int> y(2), z;

   // test capacity
   cout << "Capacity of x, y and z = "
        << ((arrayList<double>*) x)->capacity() << ", "
        << y.capacity() << ", "
        << z.capacity() << endl;


   // test size
   cout << "Initial size of x, y, and z = "
        << x->size() << ", "
        << y.size() << ", "
        << z.size() << endl;

   // test empty
   if (x->empty()) cout << "x is empty" << endl;
   else cout << "x is not empty" << endl;
   if (y.empty()) cout << "y is empty" << endl;
   else cout << "y is not empty" << endl;

   // test insert
   y.insert(0, 2);
   y.insert(1, 6);
   y.insert(0, 1);
   y.insert(2, 4);
   y.insert(3, 5);
   y.insert(2, 3);
   cout << "Inserted 6 integers, list y should be 1 2 3 4 5 6" << endl;
   cout << "Size of y = " << y.size() << endl;
   cout << "Capacity of y = " << y.capacity() << endl;
   if (y.empty()) cout << "y is empty" << endl;
   else cout << "y is not empty" << endl;
   y.output(cout);
   cout << endl << "Testing overloaded <<" << endl;
   cout << y << endl;

   // test indexOf
   int index = y.indexOf(4);
   if (index < 0) cout << "4 not found" << endl;
   else cout << "The index of 4 is " << index << endl;

   index = y.indexOf(7);
   if (index < 0) cout << "7 not found" << endl;
   else cout << "The index of 7 is " << index << endl;

   // test get
   cout << "Element with index 0 is " << y.get(0) << endl;
   cout << "Element with index 3 is " << y.get(3) << endl;

   // test erase
   y.erase(1);
   cout << "Element 1 erased" << endl;
   cout << "The list is "  << y << endl;
   y.erase(2);
   cout << "Element 2 erased" << endl;
   cout << "The list is "  << y << endl;

   cout << "Size of y = " << y.size() << endl;
   cout << "Capacity of y = " << y.capacity() << endl;
   if (y.empty()) cout << "y is empty" << endl;
   else cout << "y is not empty" << endl;   
```
```cpp
   try {
     y.insert(-3, 0);
   }  

   catch (illegalIndex e)  
   {
      cout << "Illegal index exception" << endl;
      cout << "Insert index must be between 0 and list size" << endl;
      e.outputMessage();
   }

   // test copy constructor
   arrayList<int> w(y);
   y.erase(0);
   y.erase(0);
   cout << "w should be old y, new y has first 2 elements removed" << endl;
   cout << "w is " << w << endl;
   cout << "y is " << y << endl;

   // a few more inserts, just for fun
   y.insert(0,4);
   y.insert(0,5);
   y.insert(0,6);
   y.insert(0,7);
   cout << "y is " << y << endl;
   return 0;
}

```

运行结果
>Capacity of x, y and z = 20, 2, 10  
Initial size of x, y, and z = 0, 0, 0  
x is empty  
y is empty  
Inserted 6 integers, list y should be 1 2 3 4 5 6  
Size of y = 6  
Capacity of y = 8  
y is not empty  
1  2  3  4  5  6   
Testing overloaded <<
1  2  3  4  5  6  
The index of 4 is 3  
7 not found  
Element with index 0 is 1  
Element with index 3 is 4  
Element 1 erased  
The list is 1  3  4  5  6  
Element 2 erased  
The list is 1  3  5  6  
Size of y = 4  
Capacity of y = 8  
y is not empty  
Illegal index exception  
Insert index must be between 0 and list size  
index = -3 size = 4  
w should be old y, new y has first 2 elements removed  
w is 1  3  5  6  
y is 5  6  
y is 7  6  5  4  5  6  
