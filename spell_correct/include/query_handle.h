#ifndef __QUERY_HANDLE_H
#define __QUERY_HANDLE_H
#include "excute.h"
namespace QUERY_HANDLE
{
    class Query_handle
    {
        public:
            Query_handle(const std::string& dic_path,const std::string& cache_path,LOG_SYSTEM::Log_system* log);
            void operator()(MY_SOCKET::My_socket& info);
        private:
            WORD_CORRECT::CWord_correct m_correct;
            THREADPOOL::CThread_pool m_pool;
            EXCUTE::CRun m_run;
            EXCUTE::Cache m_cache_run;
    };
}
#endif

