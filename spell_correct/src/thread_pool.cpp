#include "thread_pool.h"
namespace  THREADPOOL
{
    CThread_pool::CThread_pool(const std::string& cache_path,LOG_SYSTEM::Log_system* log):m_cache_path(cache_path),m_thread_pool(THREAD_NUM),m_que(QUEUE_CAPACITY),Cache_thread(log)
    {
        std::vector<THREAD::CThread>::iterator iter=m_thread_pool.begin();
        for(iter;iter!=m_thread_pool.end();iter++)
        {
            iter->set_log(log);
        }
    }
    void  CThread_pool::on(THREAD::CThread_RUN* thread_run,THREAD::CThread_RUN* thread_cache)
    {
        if(m_flag==false)
        {
            m_flag=true;
        }
        Cache_thread.start(thread_cache);
        std::vector<THREAD::CThread>::iterator iter=m_thread_pool.begin();
        for(iter;iter!=m_thread_pool.end();iter++)
        {
            iter->start(thread_run);
        }
    }
    void  CThread_pool::off()
    {
        if(m_flag==true)
        {
            m_flag=false;
        }
    }
    std::vector<THREAD::CThread>&  CThread_pool::get_thread_vector()
    {
        return m_thread_pool;
    }
}
