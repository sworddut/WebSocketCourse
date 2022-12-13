#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <netinet/in.h> 

int main(int argc,char** argv){
    if(argc != 4){
        printf("usage:./udpcli [ip] [port] [name]\n");
        return -1;
    }

    int sockfd = socket(PF_INET,SOCK_DGRAM,0);

    sockaddr_in dest_addr;
    memset(&dest_addr,0,sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(atoi(argv[2]));
    dest_addr.sin_addr.s_addr = inet_addr(argv[1]);

    const char* name = argv[3];

    sendto(sockfd,name,strlen(name),0,(sockaddr*)&dest_addr,sizeof(dest_addr));

    char buf[1024]{0};
    while(true){
        recvfrom(sockfd,buf,sizeof(buf),0,0,0);
        printf("%s",buf);
        memset(buf,0,sizeof(buf));
    }

    close(sockfd);
    return 0;
}
