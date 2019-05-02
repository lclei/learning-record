# stub的三种方式
使用stub是为了用我们自己的函数来替换目标文件里调用的函数，有三种方式：
- 编译时打桩
- 链接时打桩
- 运行时打桩

使用一个示例描述：用我们自己包装的mymalloc函数和myfree来替换系统的malloc和free，使其打印信息。
```c
/* mymalloc.c */
/* lcl 20190424 */
#ifdef CMPILETIME
#include <stdio.h>
#include <malloc.h>

/* malloc wrapper function */
void *mymalloc(size_t size){
    void *ptr = malloc(size);
    printf("malloc(%d)=%p\n",(int)size,ptr);
    return ptr;
}

/* free wrapper funtion */
void myfree(void *ptr){
    free(ptr);
    printf("free(%p)\n",ptr);
}
```

示例程序
```c
/* example stub */
/* int.c */
/* lcl 20190424 */
#include <stdio.h>
#include <malloc.h>

int main(){
    int *p = malloc(32);
    free(p);
    return 0;
}
```
## 编译时打桩
准备本地头文件
```c
/* malloc.h on local */
#define malloc(size) mymalloc(size)
#define free(ptr) myfree(ptr)
void *mymalloc(size_t size);
void myfree(void* ptr);
```
使用库编译mymalloc
> gcc -DCOMPILETIME -c mymalloc.c

使用本地头文件替换malloc头文件
> gcc -I . int.c mymalloc.o -o int

结果
>[root@linuxkit-00155ddc0103 stub]# ./intl
malloc(32)=0x15cb010
free(0x15cb010)

## 链接时打桩
使用链接器--wrap f选项，告诉链接器，把对f的引用解析成__wrap_f，把__real_f解析成f，从而达到打桩的目的
```c
/* mymalloc.c */
/* lcl 20190424 */
#ifdef LINKTIME
#include <stdio.h>
void *__real_malloc(size_t t);
void __real_fee(void *ptr);

/* malloc wrapper function */
void *__wrap_malloc(size_t size){
    void *ptr = __real_malloc(size);
    printf("malloc(%d)=%p\n",(int)size,ptr);
    return ptr;
}

/* free wrapper funtion */
void __wrap_free(void *ptr){
    __real_free(ptr);
    printf("free(%p)\n",ptr);
}
```                                                                   
 makefile               
```make
intl: int.o mymalloc.o                                             
    gcc -Wl,--wrap,malloc -Wl,--wrap,free int.o mymalloc.o -o intl
int.o: int.c                                                       
    gcc -c int.c                                                   
mymalloc.o: mymalloc.c                                             
    gcc -DLINKTIME -c mymalloc.c
```
结果
>[root@linuxkit-00155ddc0103 stub]# ./intl
malloc(32)=0x15cb010
free(0x15cb010)

## 运行时打桩
LD_PRELOAD
```c
#ifdef RUNTIME
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <difcn.h>

void *malloc(size_t size)
{
    void *(*mallocp)(size_t);
    char *error;

    mallocp = dlsym(RTLD_NEXT,"malloc");
    if((error = dlerror())!=NULL){
        fputs(error,stderr);
        exit(1);
    }
    char *ptr = mallocp(size);
    printf("malloc(%d) = %p\n",(int)size, ptr);
    return ptr;
}

void free(void *ptr){
    void (*freep)(void *) = NULL;
    char *error;
    if(!ptr) return;

    freep = dlsym(RTLD_NEXT,"free");
    if ((error=dlerror())!=NULL){
        fputs(error,stderr);
        exit(1);
    }
    freep(ptr);
    printf("free(%p)\n",ptr);
}
#endif
```
编译
```make
intr: int.c
    gcc -DRUNTIME -shared -fpic mymalloc.c -ldl -o mymalloc.so
    gcc int.c -o intr
 ```
 结果
>[root@linuxkit-00155ddc0103 stub]# LD_PRELOAD="./mymalloc.so" ./intr  
>malloc(32) = 0x11eb010  
>free(0x11eb010)

甚至还可以对其他程序应用
>[root@linuxkit-00155ddc0103 stub]# LD_PRELOAD="./mymalloc.so" date  
malloc(34) = 0x2225010  
malloc(10) = 0x2225040  
malloc(15) = 0x2225060  
malloc(568) = 0x2225080  
malloc(29) = 0x22252c0  
free(0x2225080)  
malloc(20) = 0x2225080  
Thu Apr 25 12:53:41 UTC 2019
