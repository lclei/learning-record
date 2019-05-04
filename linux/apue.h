#ifndef _APUE_H
#define _APUE_H
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/termios.h>

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define MAXLINE 4096
#define FILE_MODE (S_IRUSR| S_IWUSR| S_IRGRP| S_IROTH)
#define DIR_MODE (FILE_MODE | S_IXUDR | S_IXGRP | S_IXOTH)

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

#endif
