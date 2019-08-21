#include "mynet.h"

#define SERV_PORT 18529
#define IP "127.0.0.1"

int main(int argc, char* argv[])
{
    int port = SERV_PORT;
    struct sockaddr_in servaddr, clieaddr;
    int lfd, cfd, n;
    char buf[BUFSIZ];
    socklen_t clie_addr_len;

    if(argc>1)
    {
        port = atoi(argv[1]);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    lfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(lfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(lfd,128);
    clie_addr_len = sizeof(clieaddr);
    printf("listening...\n");
    cfd = accept(lfd,(struct sockaddr*)&clieaddr, &clie_addr_len);
    n = read(cfd, buf, BUFSIZ);
    for(int i=0; i<n; i++)
    {
        buf[i] = toupper(buf[i]);
    }
    write(cfd,buf,n);
    close(cfd);
    close(lfd);
    return 0;
}
