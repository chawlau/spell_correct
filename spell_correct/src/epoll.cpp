#include"epoll.h"
namespace MY_EPOLL
{
    int  My_epoll::set_nonblock(int sockfd)
    {
        int old=fcntl(sockfd,F_GETFL);
        old|=O_NONBLOCK;
        fcntl(sockfd,F_SETFL,old);
        return 0;
    }
    void My_epoll::socket_init()
    {
        server_sock=socket(AF_INET,SOCK_DGRAM,0);
        if(server_sock==-1)
        {
            m_log.error("server sock create failed");
        }
        memset(&server_addr,0,sizeof(server_addr));
        server_addr.sin_family=AF_INET;
        server_addr.sin_port=htons(m_port);
        server_addr.sin_addr.s_addr=inet_addr(m_ip.c_str());
        if(bind(server_sock,(struct sockaddr*)&server_addr,sizeof(server_addr))==-1)
        {
            m_log.error("server socket binding failed");
        }
    }
    int  My_epoll::recv_msg(char* message)
    {
        memset(message,0,MAXSIZE);
        int sum=recvfrom(server_sock,message,MAXSIZE,0,(struct sockaddr*)&client_addr,&client_addr_len);
        std::string temp1,temp2;
        std::stringstream ss;
        if(sum==-1)
        {
            temp1="recv msg error from client(IP";
            ss<<inet_ntoa(client_addr.sin_addr)<<":"<<ntohs(client_addr.sin_port)<<")";
            ss>>temp2;
            m_log.error(temp1+temp2);
        }
        temp1="server recv query from client(IP"; 
        ss<<inet_ntoa(client_addr.sin_addr)<<":"<<ntohs(client_addr.sin_port)<<")";
        ss>>temp2;
        m_log.notice(temp1+temp2);
        return sum;
    }
    void My_epoll::handle_query()
    {
        struct epoll_event events[MAXSIZE];
        int epollfd=epoll_create(MAXSIZE);
        if(epollfd==-1)
        {
            m_log.error("epoll create failed");
        }
        socket_init();
        set_nonblock(server_sock);
        QUERY_HANDLE::Query_handle handle(m_dic_path,m_cache_path,&m_log);
        epoll_add(epollfd,server_sock,EPOLLIN|EPOLLET);
        std::cout<<"server wait for connecting......."<<std::endl;
        m_log.notice("server wait for connecting.........");
        while(1)
        {
            int ret=epoll_wait(epollfd,events,MAXSIZE,-1);
            for(int index=0;index<ret;index++)
            {
                if(events[index].data.fd==server_sock)
                {
                    char msg[MAXSIZE]="";
                    recv_msg(msg);
                    MY_SOCKET::My_socket info(client_addr,client_addr_len,msg);
                    handle(info);
                }
            }
        }
    }
    void My_epoll::epoll_add(int epollfd,int sockfd,int state)
    {
        struct epoll_event event;
        event.events=state;
        event.data.fd=sockfd;
        epoll_ctl(epollfd,EPOLL_CTL_ADD,sockfd,&event);
    }
}
