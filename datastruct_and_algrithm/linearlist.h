#ifndef _LINEARLIST_H
#define _LINEARLIST_H

#include <iostream>
#include <algorithm>
#include <string>
#include "exception.h"
using namespace std;

template<class T>
class linearList
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

// array
template<class T>
class arrayList : public linearList<T>
{
public:
  // 构造函数，复制构造函数和析构函数
  arrayList(int initialCapacity = 10);
  arrayList(const arrayList<T>&);
  ~arrayList() {delete [] element;}

  // ADT 方法
  bool empty() const {return listSize == 0;}
  int size() const {return listSize;}
  T& get(int theIndex) const;
  int indexOf(const T& theElement) const;
  void erase(int theIndex);
  void insert(int theIndex, const T& theElement);
  void output(ostream& out) const;

  // other function
  int capacity() const {return arrayLength;}

protected:
  void checkIndex(int theIndex) const;
  T* element;
  int arrayLength;
  int listSize;
}

template<class T>
void changeLength1D(T*& a, int oldLength, int newLength){
  if(newLength < 0)
    throw illegalParameterValue("new length must be >= 0");

  T* temp = new T[newLength];
  int number = min(oldLength, newLength);
  copy(a, a+number, temp);
  delete [] a;
  a = temp;
}
// construct function
template<class T>
arrayList<T>::arrayList(int initialCapacity){
  if (initialCapacity<1){
    ostream s;
    s << "initial capacity = " << initialCapacity << " must be > 0";
    throw illegalParameterValue(s.str());
  }
  arrayLength = initialCapacity;
  element = new T[arrayLength];
  listSize = 0;
}

// copy construct function
template<class T>
arrayList<T>::arrayList(const arrayList<T>& theList){
  arrayLength = theList.arrayLength;
  lisrSize = theList.listSize;
  element = new T[arrayLength];
  copy(theList.element, theList.listSize, element);
}

template<class T>
void arrayList<T>::checkIndex(int theIndex) const{
  if(theIndex<0 || theIndex>=listSize){
    ostream s;
    s << "index = " << theIndex << " size = " << listSize;
    throw illegalParameterValue(s.str());
  }
}

template<class T>
T& arrayList<T>::get(int theIndex) const {
  checkIndex(theIndex);
  return element[theIndex];
}

template<class T>
int arrayList<T>::indexOf(const T& theElement) const{
  int theIndex = (int)(find(element, element + listSize, theElement) - element);
  if (theIndex == listSize) return -1;
  else return theIndex;
}

template<class T>
void arrayList<T>::erase(int theIndex){
  checkIndex(theIndex);
  copy(element + theIndex+1, element + listSize, element + theIndex);
  element[--listSize].~T();
}

template<class T>
void arrayList<T>::insert(int theIndex, const T& theElement){
  checkIndex(theIndex+1);

  if(listSize == arrayLength){
    changeLength1D(element, arrayLength, 2*arrayLength);
    arrayLength *= 2;
  }

  copy_backward(element + theIndex, element + listSize, element + theIndex + 1);
  element[theIndex] = theElement;
  listSize++;
}

template<class T>
void arrayList<T>::output(ostream& out) const
{// Put the list into the stream out.
   copy(element, element + listSize, ostream_iterator<T>(cout, "  "));
}

// overload <<
template <class T>
ostream& operator<<(ostream& out, const arrayList<T>& x)
   {x.output(out); return out;}

#endif
