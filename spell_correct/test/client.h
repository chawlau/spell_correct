#ifndef __CLIENT_
#define __CLIENT_
#include<cstdio>
#include<iostream>
#include<cstdlib>
#include<string>
#include<cstring>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<sys/types.h>
#include<fcntl.h>
#include<pthread.h>
namespace MY_CLIENT
{
    class My_client
    {
        public:
            int server_sock;
            struct sockaddr_in server_addr;
            char message[1024];
            socklen_t server_addr_len;
            std::string m_ip;
            int m_port;
            My_client(std::string ip,int port,socklen_t server_adr_len=sizeof(server_addr)):m_ip(ip),m_port(port),server_addr_len(server_adr_len){}
            void socket_init();
            int recv_msg();
            int send_msg(char* msg);
    };
}
#endif
