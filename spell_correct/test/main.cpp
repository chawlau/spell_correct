#include "client.h"
int main(int argc,char* argv[])
{
    MY_CLIENT::My_client client(argv[1],atoi(argv[2]));   
    client.socket_init();   
    char msg[1024];
    while(memset(msg,0,1024),std::cout<<"请输入查询结果"<<std::endl,std::cin>>msg)
    {
        client.socket_init();   
        client.send_msg(msg);
        client.recv_msg();
    }
}
