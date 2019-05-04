# 守护进程

- 生存期长，常常在系统引导装入时启动，仅在系统关闭时终止
- 没有控制终端

## 如何编写
1. umask(0)
2. 调用fork，使父进程exit
3. setsid创建一个新会话
4. 工作目录改为根目录
5. 关闭不需要的文件描述符
6. 关闭标准输入输出

实例

```c
#include "apue.h"
#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>

void deamonize(const char *cmd){
  int i, fd0, fd1, fd2;
  pid_t pid;
  struct rlimit rl;
  struct  sigaction sa;

  // Clear file creation umask
  umask(0);

  // Get maximum number of file descriptors
  if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
    err_quit("%s: can't get file limt", cmd);

  // Become a session leader to lose controlling TTY
  if( (pid=fork()) < 0)
    err_quit("%s: can't get file limit", cmd);
  else if (pid != 0) // Parent
    exit(0);
  setsid();

  // Ensure future opens won't allocate controlling TTYs
  sa.sa_handler = SIG_IGN;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  if (sigaction(SIGHUP, &sa, NULL) < 0)
    err_quit("%s: can't ignore SIGHUP", cmd);
  if ((pid = fork()) < 0)
    err_quit("%s: can't fork", cmd);
  else if (pid != 0)
    exit(0);

  // Change the cuurent working directory to the root so
  // we won't prevent file systems from being unmounted.

  if (chdir("/") < 0)
    err_quit("%s: can't change directory to /", cmd);

  // Close all the file descriptors
  if(rl.rlim_max == RLIM_INFINITY)
    rl.rlim_max = 1024;
  for(i = 0;i <rl.lim_max;i++)
    close(i);

  // Attach file descriptors 0, 1, 2 to /dev/ull
  fd0 = open("/dev/null", O_RDWR);
  fd1 = dup(0);
  fd2 = dup(0);

  // Initiallize the log file
  openlog(cmd, LOG_CONS, LOG_DAEMON);
  if(fd0!=0 || fd1 != 1 || fd2 != 2){
    syslog(LOG_ERR, "unexpected file descriptors %d %d %d", fd0, fd1, fd2);
    exit(1);
  }


}
```
下面使用daemonize将main函数变为守护进程
```
void daemonize(const char *cmd);
int main(int argc, char **argv){
  daemonize("main");
  sleep(110);
}
```
查看执行结果
```
lcl@FX50JK:~/Documents/hub/learning-record/linux/daemon$ ps -efj | grep ./main
lcl      23738  1542 23737 23737  0 17:13 ?        00:00:00 ./main
lcl      23744  6959 23743  6959  0 17:14 pts/1    00:00:00 grep --color=auto ./main

```
与一个正常非守护进程main1对比
```
lcl@FX50JK:~/Documents/hub/learning-record/linux/daemon$ ps -efj | grep ./main
UID        PID  PPID  PGID   SID  C STIME TTY          TIME CMD
lcl      24079  1542 24078 24078  0 18:17 ?        00:00:00 ./main
lcl      24089  2166 24089  2166  0 18:18 pts/0    00:00:00 ./main1
```
可见守护进程没有绑定终端，在孤儿进程祖，不是会话首进程
