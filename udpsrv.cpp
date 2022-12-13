#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <list>
#include <netinet/in.h> 

int main(int argc,char** argv){
    if(argc != 2){
        printf("usage:./udpsrv [port]\n");
        return -1;
    }

    std::list<sockaddr*> sockaddr_list;

    int sockfd = socket(PF_INET,SOCK_DGRAM,0);

    sockaddr_in socket_addr;
    memset(&socket_addr,0,sizeof(socket_addr));
    socket_addr.sin_family = AF_INET;
    socket_addr.sin_port = htons(atoi(argv[1]));
    socket_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sockfd,(sockaddr*)&socket_addr,sizeof(socket_addr));

    char recv_buf[1024]{0},send_buf[1024]{0};

    while(true){
        sockaddr* dest_addr = (sockaddr*)malloc(sizeof(sockaddr_in));
        socklen_t socklen = sizeof(sockaddr_in);
        recvfrom(sockfd,recv_buf,sizeof(recv_buf),0,dest_addr,&socklen);
        sockaddr_list.push_back(dest_addr);
        
        in_addr cli_ip;
        cli_ip.s_addr = reinterpret_cast<sockaddr_in*>(dest_addr)->sin_addr.s_addr;
        const char* ip = inet_ntoa(cli_ip);
        int port = reinterpret_cast<sockaddr_in*>(dest_addr)->sin_port;
        snprintf(send_buf,sizeof(send_buf),"new user login,name - %s,ip - %s,port - %d\n",recv_buf,ip,port);
        printf("%s",send_buf);

        for(auto i : sockaddr_list)
            sendto(sockfd,send_buf,strlen(send_buf),0,i,sizeof(sockaddr_in));
        

        memset(recv_buf,0,sizeof(recv_buf));
        memset(send_buf,0,sizeof(send_buf));
    }

    close(sockfd);
    return -1;
}
