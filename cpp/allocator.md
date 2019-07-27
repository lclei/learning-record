#allocator

>\#include \<memory>

>allocator\<T> a
定义一个名为a的allocator对象，它可以为类型为T的对象分配内存

>a.allocate(n)
分配一段原始的、未构造的内存，保存n个类型为T的对象

>a.deallocate(p,n)
释放从T*指针p中地址开始的内存，这块内存保存了n个类型为T的对象，与a.allocate(n)对应

>a.construct(p, args)
p是指针，args是对象的构造函数，用来构造对象

>a.destroy(p)
对p指向的对象执行析构函数
