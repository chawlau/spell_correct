#include "query_handle.h"
namespace QUERY_HANDLE
{
    Query_handle::Query_handle(const std::string& dic_path,const std::string& cache_path,LOG_SYSTEM::Log_system* log):m_correct(dic_path.c_str()),m_pool(cache_path,log),m_run(&m_pool.m_que),m_cache_run(m_pool)
    {
        m_pool.on(&m_run,&m_cache_run);
    }
    void Query_handle::operator()(MY_SOCKET::My_socket& info)
    {
        EXCUTE::CTask_excute* ptr=new EXCUTE::CTask_excute(m_correct,info);
        m_pool.m_que.produce(ptr);
    }
}

