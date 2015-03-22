#ifndef __EPOLL_H
#define __EPOLL_H
#include <iostream>
#include "thread_pool.h"
#include "socket.h"
#include "conf.h"
#include "query_handle.h"
namespace MY_EPOLL
{
    class My_epoll
    {
        public:
            My_epoll(const std::string& conf_path,socklen_t client_adr_len=sizeof(client_addr)):m_conf(conf_path),m_ip(m_conf.config["my_ip"]),m_port(atoi(m_conf.config["my_port"].c_str())),m_dic_path(m_conf.config["my_dic"]),m_cache_path(m_conf.config["my_cache"]),client_addr_len(client_adr_len),m_log(m_conf.config["my_log"].c_str())
        {
            m_log.init(); 
        }
            void socket_init();
            int recv_msg(char* msg);
            void handle_query();
            int  set_nonblock(int sockfd);
            void epoll_add(int epollfd,int sockfd,int state);
            char message[MAXSIZE];
        private:
            MY_CONF::CMyconf m_conf;
            int server_sock;
            struct sockaddr_in server_addr,client_addr;
            socklen_t client_addr_len;
            const std::string m_ip;
            const int m_port;
            const std::string m_dic_path;
            const std::string m_cache_path;
            LOG_SYSTEM::Log_system m_log;
            My_epoll& operator=(My_epoll& obj);
            My_epoll(My_epoll& obj);
    };
}
#endif
