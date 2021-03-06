#include "../apue.h"
#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>

void daemonize(const char *cmd){
  int i, fd0, fd1, fd2;
  pid_t pid;
  struct rlimit rl;
  struct  sigaction sa;

  /*
   * Clear file creation umask
   */
  umask(0);

  // Get maximum number of file descriptors
  if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
    printf("%s: can't get file limt", cmd);

  // Become a session leader to lose controlling TTY
  if( (pid=fork()) < 0)
    printf("%s: can't get file limit", cmd);
  else if (pid != 0) // Parent
    exit(0);
  setsid();

  // Ensure future opens won't allocate controlling TTYs
  sa.sa_handler = SIG_IGN;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  if (sigaction(SIGHUP, &sa, NULL) < 0)
    printf("%s: can't ignore SIGHUP", cmd);
  if ((pid = fork()) < 0)
    printf("%s: can't fork", cmd);
  else if (pid != 0)
    exit(0);

  // Change the cuurent working directory to the root so
  // we won't prevent file systems from being unmounted.

  if (chdir("/") < 0)
    printf("%s: can't change directory to /", cmd);

  // Close all the file descriptors
  if(rl.rlim_max == RLIM_INFINITY)
    rl.rlim_max = 1024;
  for(i = 0;i <rl.rlim_max;i++)
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
