# arrayListWithIterator

添加iterator类和begin()，end()

- 迭代器类型定义迭代器的分类。这些标签都是空类型，分别对应五种迭代器分类中的一种，迭代器类型用于指明实例化迭代器的标签，如:输入迭代器(写)、输出迭代器(读)、随机访问迭代器等，算法可通过不同迭代器种类来实现各版本。  
input_iterator_tag 对应 LegacyInputIterator 。  
output_iterator_tag 对应 LegacyOutputIterator 。  
forward_iterator_tag 对应 LegacyForwardIterator 。  
bidirectional_iterator_tag 对应 LegacyBidirectionalIterator 。  
random_access_iterator_tag 对应 LegacyRandomAccessIterator

```cpp
template<class T>
class arrayList : public linearList<T>
{
   public:
     ...

      // iterators to start and end of list
      class iterator;
      iterator begin() {return iterator(element);}
      iterator end() {return iterator(element + listSize);}

      // iterator for arrayList
      class iterator
      {
         public:
            // typedefs required by C++ for a bidirectional iterator
            typedef bidirectional_iterator_tag iterator_category;
            typedef T value_type;
            typedef ptrdiff_t difference_type;
            typedef T* pointer;
            typedef T& reference;

            // constructor
            iterator(T* thePosition = 0) {position = thePosition;}

            // dereferencing operators
            T& operator*() const {return *position;}
            T* operator->() const {return &*position;}

            // increment
            iterator& operator++()   // preincrement
                      {++position; return *this;}
            iterator operator++(int) // postincrement
            	      {iterator old = *this;
            	       ++position;
            	       return old;
            	      }

            // decrement
            iterator& operator--()   // predecrement
                      {--position; return *this;}
            iterator operator--(int) // postdecrement
            	      {iterator old = *this;
            	       --position;
            	       return old;
            	      }

            // equality testing
            bool operator!=(const iterator right) const
                  {return position != right.position;}
            bool operator==(const iterator right) const
                  {return position == right.position;}
         protected:
            T* position;
      };  // end of iterator class

   protected:  // additional members of arrayList
      void checkIndex(int theIndex) const;
            // throw illegalIndex if theIndex invalid
      T* element;            // 1D array to hold list elements
      int arrayLength;       // capacity of the 1D array
      int listSize;          // number of elements in list
};
```
应用实例
```cpp
// test the iterator for arrayList
#include<iostream>
#include<algorithm>   // has reverse
#include<numeric>     // has accumulate
#include "linearList.h"
#include "arrayListWithIterator.h"

using namespace std;

int main()
{
   // create a linear list
   arrayList<int> y(2);
   y.insert(0, 2);
   y.insert(1, 6);
   y.insert(0, 1);
   y.insert(2, 4);
   y.insert(3, 5);
   y.insert(2, 3);
   cout << "Inserted 6 integers, list y should be 1 2 3 4 5 6" << endl;
   cout << "Size of y = " << y.size() << endl;
   cout << "Capacity of y = " << y.capacity() << endl;

   // test iterator
   cout << "Ouput using forward iterators pre and post ++" << endl;
   for (arrayList<int>::iterator i = y.begin();
        i != y.end(); i++)
      cout << *i << "  ";
   cout << endl;
   for (arrayList<int>::iterator i = y.begin();
        i != y.end(); ++i)
      cout << *i << "  ";
   cout << endl;

   cout << "Ouput using backward iterators pre and post --" << endl;
   for (arrayList<int>::iterator i = y.end();
        i != y.begin(); cout << *(--i) << "  ");
   cout << endl;
   for (arrayList<int>::iterator i = y.end();
        i != y.begin();)
      {i--; cout << *i << "  "; *i += 1;}
   cout << endl;
   cout << "Incremented by 1 list is " << y << endl;

   // try out some STL algorithms
   reverse(y.begin(), y.end());
   cout << "The reversed list is " << y << endl;
   int sum = accumulate(y.begin(), y.end(), 0);
   cout << "The sum of the elements is " << sum << endl;

   return 0;
}

```
output
```
lcl@FX50JK:~/Documents/hub/learning-record/datastruct_and_algrithm$ ./arrayListWithIterator
Inserted 6 integers, list y should be 1 2 3 4 5 6
Size of y = 6
Capacity of y = 8
Ouput using forward iterators pre and post ++
1  2  3  4  5  6  
1  2  3  4  5  6  
Ouput using backward iterators pre and post --
6  5  4  3  2  1  
6  5  4  3  2  1  
Incremented by 1 list is 2  3  4  5  6  7  
The reversed list is 7  6  5  4  3  2  
The sum of the elements is 27
```
