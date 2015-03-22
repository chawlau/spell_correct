#ifndef __FUNC_H
#define __FUNC_H
#include "socket.h"
#include "thread_pool.h"
namespace EXCUTE
{
    class CTask_excute:public QUEUE::CQueue::CTask
    {
        public:
            CTask_excute(WORD_CORRECT::CWord_correct& correct,MY_SOCKET::My_socket& info);
            std::string excute(hash_record* m_cache,LOG_SYSTEM::Log_system* log);
            int send_msg(const char* msg,LOG_SYSTEM::Log_system* log);
            void socket_init(LOG_SYSTEM::Log_system* log);
        private:
            WORD_CORRECT::CWord_correct& m_correct;
            MY_SOCKET::My_socket& m_info;
            int client_sock;
    };
    class CRun:public THREAD::CThread_RUN
    {
        public:
            CRun(QUEUE::CQueue* pq);
            void run();
            void set_log(LOG_SYSTEM::Log_system* log);
            void set_cache(hash_record* cache);
        private:
            QUEUE::CQueue* m_pq;
            hash_record* m_cache;
            LOG_SYSTEM::Log_system* m_log;
    };
    class Cache:public THREAD::CThread_RUN
    {
        public:
            Cache(THREADPOOL::CThread_pool& pool);
            void run();
            void set_cache(hash_record* cache);
            void set_log(LOG_SYSTEM::Log_system* log);
            void cache_out();
            void read_disk_cache();
            void write_disk_cache();
        private:
            hash_record* m_cache;
            const std::string& m_cache_path;
            std::vector<THREAD::CThread>& m_thread_vector;
            LOG_SYSTEM::Log_system* m_log; 
    };
}
#endif
