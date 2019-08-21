#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include "wrap.h"

#define SERV_PORT 18529
#define IP "127.0.0.1"
#define MAXLINE 10

int main(int argc, char* argv[])
{
    int port = SERV_PORT;
    struct sockaddr_in servaddr, clieaddr;
    int lfd, cfd, efd,  n;
    char buf[MAXLINE];
    char str[INET_ADDRSTRLEN];
    socklen_t clie_addr_len;

    if(argc>1)
    {
        port = atoi(argv[1]);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    lfd = Socket(AF_INET, SOCK_STREAM, 0);
    Bind(lfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(lfd,128);
    //////////////////////////////////////////
    struct epoll_event event;
    struct epoll_event resevent[MAXLINE];
    int res,len,flag;

    efd = epoll_create(MAXLINE);
    event.events = EPOLLIN | EPOLLET;
    clie_addr_len = sizeof(clieaddr);
    printf("listening...\n");
    cfd = Accept(lfd,(struct sockaddr*)&clieaddr, &clie_addr_len);
    printf("received froom %s at PORT %d\n",
        inet_ntop(AF_INET, &clieaddr.sin_addr, str, sizeof(str)),
        ntohs(clieaddr.sin_port));

    flag = fcntl(cfd, F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(cfd, F_SETFL, flag);

    event.data.fd = cfd;
    epoll_ctl(efd, EPOLL_CTL_ADD, cfd, &event);

    while(1) {
        res = epoll_wait(efd, resevent, 10, -1);

        if(resevent[0].data.fd == cfd){
            while((len = read(cfd, buf, MAXLINE/2)) > 0)
                write(STDOUT_FILENO, buf, len);
        }

    }
    return 0;
}
