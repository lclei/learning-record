# 高级IO
+ 非阻塞IO
+ 记录锁
+ I/O 多路转接(select and poll)
+ 异步IO
+ readv, writev 和 mmap

## 非阻塞IO
阻塞的情况
- 某些文件类型（读管道、终端设备和网络设备）不存在，读操作调用者可能被永远阻塞
- 数据不能被相同的文件类型立即接受（如管道中无空间、网络流控制），写操作调用者可能被永远阻塞
- 在某种条件发生前卡开某些文件类型
- 对已经加上强制性记录锁的文件进行读写
- 某些ioctl操作
- 某些进程间通信函数

对于一个指定的文件描述符，有两种指定为非阻塞I/O的方法
1. 如果调用open获得描述符，可指定O_NONBLOCK标志
2. 对于已经打开的一个描述符，则可以调用fcntl，又该函数打开O_NONBLOCK文件状态标志

下面的例子将从标准输入读入500000byte内容，然后将标准输出设置(set_fl)为nonblock，通过循环将内容输出到标准输出，把日志输出到stderr，最后把清除该标志(clr_fl)

```c
#include "../apue.h"
#include <errno.h>
#include <fcntl.h>

void set_fl(int fd, int flags);
void clr_fl(int fd, int flags);

char buf[500000];

int main(void){
  int ntowrite, nwrite;
  char *ptr;

  ntowrite = read(STDIN_FILENO, buf, sizeof(buf));
  fprintf(stderr, "read %d bytes\n", ntowrite);

  set_fl(STDOUT_FILENO, O_NONBLOCK);

  ptr = buf;
  while(ntowrite > 0 ){
    errno = 0;
    nwrite = write(STDOUT_FILENO, ptr, ntowrite);
    fprintf(stderr, "nwrite = %d, errno = %d\n", nwrite, errno);

    if(nwrite > 0){
      ptr += nwrite;
      ntowrite -= nwrite;
    }
  }

  clr_fl(STDOUT_FILENO, O_NONBLOCK);

  exit(0);
}

void set_fl(int fd, int flags){
  int val;

  if((val = fcntl(fd, F_GETFL, 0)) < 0){
    printf("fcntl F_GETFL error\n" );
    exit(0);
  }

  val |= flags;

  if (fcntl(fd, F_SETFL, val) < 0){
    printf("fcntl F_SETFL error\n" );
    exit(0);
  }
}

void clr_fl(int fd, int flags){
  int val;

  if((val = fcntl(fd, F_GETFL, 0)) < 0){
    printf("fcntl F_GETFL error\n" );
    exit(0);
  }

  val &= ~flags;

  if (fcntl(fd, F_SETFL, val) < 0){
    printf("fcntl F_SETFL error\n" );
    exit(0);
  }
}

```
作为结果检验
1. 标准输入重定位到一个大文件
```bash
lcl@FX50JK:~/Documents/hub/learning-record/linux/advancedIO$ ls -l ~/Downloads/code_book/shujujiegouc++.pdf
-rwxrwxrwx 1 lcl lcl 17599970 9月  14  2018 /home/lcl/Downloads/code_book/
shujujiegouc++.pdf
```
2. 输出重定位到一个文件，结果正常
```bash
lcl@FX50JK:~/Documents/hub/learning-record/linux/advancedIO$ ./nonblock < ~/Downloads/code_book/shujujiegouc++.pdf >temp.file
-rw-r--r-- 1 lcl lcl 500000 5月   4 21:51 temp.file
read 500000 bytes
nwrite = 500000, errno = 0
```
3. 输出到终端，出错信息重定位到stderr.out，可以看到出错码为11，意思是
>errno11 :    Resource temporarily unavailable

  ```bash
./nonblock < ~/Downloads/code_book/shujujiegouc++.pdf 2>stderr.out
cat stderr.out
nwrite = -1, errno = 11
nwrite = -1, errno = 11
nwrite = -1, errno = 11
nwrite = -1, errno = 11
nwrite = -1, errno = 11
nwrite = -1, errno = 11
nwrite = -1, errno = 11
nwrite = -1, errno = 11
nwrite = -1, errno = 11
nwrite = -1, errno = 11
nwrite = -1, errno = 11
nwrite = -1, errno = 11
nwrite = 135529, errno = 0
```

因此非阻塞的程序使用循环读取时会一直占用资源
