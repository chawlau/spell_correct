#include "epoll.h"
int main(int argc,char* argv[])
{
    MY_EPOLL::My_epoll server(argv[1]);
    std::cout<<"server system init"<<std::endl;
    server.handle_query();
}
