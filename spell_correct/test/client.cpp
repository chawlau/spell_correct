#include"client.h"
namespace MY_CLIENT
{
    void My_client::socket_init()
    {
        server_sock=socket(AF_INET,SOCK_DGRAM,0);
        if(server_sock==-1)
        {
            perror("server sock create failed");
        }
        memset(&server_addr,0,sizeof(server_addr));
        server_addr.sin_family=AF_INET;
        server_addr.sin_port=htons(m_port);
        server_addr.sin_addr.s_addr=inet_addr(m_ip.c_str());
    }
    int  My_client::recv_msg()
    {
        memset(message,0,1024);
        /*
        if(recvfrom(server_sock,message,1024,0,(struct sockaddr*)&server_addr,&server_addr_len)==-1)
        {
            perror("recv message");
        }
        std::cout<<"recv "<<message<<" from server"<<std::endl;
        */
        int recvlen=0;
        if(recvfrom(server_sock,(char*)&recvlen,4,0,(struct sockaddr*)&server_addr,&server_addr_len)==-1)
        {
            perror("recv length");
        }
        int ret=0,sum=0;
        while(sum<recvlen)
        {
            ret=recvfrom(server_sock,message+sum,recvlen-sum,0,(struct sockaddr*)&server_addr,&server_addr_len);
            sum+=ret;
        }
        std::cout<<message;
        //std::cout<<"recv :"<<message<<"  from server( IP "<<inet_ntoa(server_addr.sin_addr)<<":"<<ntohs(server_addr.sin_port)<<" )"<<std::endl;
        return sum;
    }
    int My_client::send_msg(char* msg) 
    {
        int sum=sendto(server_sock,msg,strlen(msg),0,(struct sockaddr*)&server_addr,server_addr_len);
        if(sum==-1)
        {
            perror("send message");
        }
        return sum;
        /*
        int sendlen=strlen(msg);
        if(sendto(server_sock,(char*)&sendlen,4,0,(struct sockaddr*)&server_addr,server_addr_len)==-1)
        {
            perror("send length");
        }
        int ret=0,sum=0;
        while(sum<sendlen)
        {
            ret=sendto(server_sock,msg+sum,sendlen-sum,0,(struct sockaddr*)&server_addr,server_addr_len);
            sum+=ret;
        }
        return sum;*/
    }
}
