#include"socket.h"
namespace MY_SOCKET
{
    My_socket::My_socket(struct sockaddr_in& client_addr,socklen_t& client_addr_len,char* msg):m_client_addr(client_addr),m_client_addr_len(client_addr_len)
    {
        memset(m_search,0,MAXSIZE);
        strcpy(m_search,msg);
    }
}
