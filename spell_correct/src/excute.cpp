#include"excute.h"
namespace EXCUTE
{
    CTask_excute::CTask_excute(WORD_CORRECT::CWord_correct& correct,MY_SOCKET::My_socket& info):m_correct(correct),m_info(info){}
    std::string CTask_excute::excute(hash_record* m_cache,LOG_SYSTEM::Log_system* m_log)
    {
        std::string result;
        if((*m_cache).count(m_info.m_search)>0)
        {
            m_log->notice("run thread's hash_map cache has result,return answer to client directly");
            result=(*m_cache)[m_info.m_search];
        }
        else
        {
            m_log->notice("run thread's hash_map cache no result,go to hash_map index of dic for answer");
            result=m_correct(m_info.m_search);
            std::pair<std::string,std::string> apair=make_pair(m_info.m_search,result);
            (*m_cache).insert(apair);
        }
        socket_init(m_log);
        send_msg(result.c_str(),m_log);
        return result;
    }
    int CTask_excute::send_msg(const char* msg,LOG_SYSTEM::Log_system* m_log) 
    {
        std::string temp1,temp2;
        std::stringstream ss;
        int sendlen=strlen(msg);
        if(sendto(client_sock,(char*)&sendlen,4,0,(struct sockaddr*)&m_info.m_client_addr,m_info.m_client_addr_len)==-1)
        {
            temp1="send msg error to client(IP";            
            ss<<inet_ntoa(m_info.m_client_addr.sin_addr)<<":"<<ntohs(m_info.m_client_addr.sin_port)<<")";
            ss>>temp2;
            m_log->error(temp1+temp2);
        }
        int ret=0,sum=0;
        while(sum<sendlen)
        {
            ret=sendto(client_sock,msg+sum,sendlen-sum,0,(struct sockaddr*)&m_info.m_client_addr,m_info.m_client_addr_len);
            sum+=ret;
        }
            temp1="send msg successful to client(IP";
            ss<<inet_ntoa(m_info.m_client_addr.sin_addr)<<":"<<ntohs(m_info.m_client_addr.sin_port)<<")";
            ss>>temp2;
            m_log->notice(temp1+temp2);
        return sum;
    }
    void CTask_excute::socket_init(LOG_SYSTEM::Log_system* m_log)
    {
        client_sock=socket(AF_INET,SOCK_DGRAM,0);
        if(client_sock==-1)
        {
            m_log->error("server sock create failed");
        }
    }
    CRun::CRun(QUEUE::CQueue* pq):m_pq(pq){}
    void CRun::run()
    {
        while(1)
        {
            QUEUE::CQueue::CPtask task;
            if(m_pq->consume(&task)==false)
            {
                m_log->error("consume task from queue false");
            }
            task->excute(m_cache,m_log);
            sleep(rand()%3+1);
        }
    }
    void CRun::set_cache(hash_record* cache)
    {
        m_log->notice("set run thread's hash_map cache");
        m_cache=cache;
    }
    void CRun::set_log(LOG_SYSTEM::Log_system* log)
    {
        m_log=log;
        m_log->notice("set run thread log_system");
    }
    Cache::Cache(THREADPOOL::CThread_pool& pool):m_cache_path(pool.m_cache_path),m_thread_vector(pool.get_thread_vector()){}
    void Cache::read_disk_cache()
    {
        std::fstream fin(m_cache_path.c_str());
        std::string word;
        while(getline(fin,word))
        {
            size_t start=word.find(" ");
            std::string key=word.substr(0,start);
            std::string value=word.substr(start+1);
            std::pair<std::string,std::string> apair=make_pair(key,value);
            (*m_cache).insert(apair);
        }
        m_log->notice("disk cache read is over!");
    }
    void Cache::write_disk_cache()
    {
        std::ofstream fout(m_cache_path.c_str());
        piter iter=(*m_cache).begin();
        for(iter;iter!=(*m_cache).end();iter++)
        {
            fout<<iter->first<<" "<<iter->second<<std::endl;
        }
        m_log->notice("disk cache write is over!");
    }
    void Cache::run()
    {
        read_disk_cache();
        while(1)
        {
            m_log->notice("cache synchronize start!");
            std::vector<THREAD::CThread>::iterator iter=m_thread_vector.begin();
            for(iter;iter!=m_thread_vector.end();iter++)
            {
                piter iter1=(*(iter->get_record())).begin();
                for(iter1;iter1!=(*(iter->get_record())).end();iter1++)
                {
                    std::pair<std::string,std::string> apair=make_pair(iter1->first,iter1->second);
                    (*m_cache).insert(apair);
                }
            }
            m_log->notice("cache thread syschronize thread_pool's hash_map");
            for(iter;iter!=m_thread_vector.end();iter++)
            {
                hash_record& temp=(*(iter->get_record()));
                temp.clear();
                piter iter1=(*m_cache).begin();
                for(iter1;iter1!=(*m_cache).end();iter++)
                {
                    std::pair<std::string,std::string> apair=make_pair(iter1->first,iter1->second);
                    temp.insert(apair);
                }
            }
            m_log->notice("cache thread syschronize result back to thread_pool");
            write_disk_cache();
            m_log->notice("synchronize over!");
            sleep(10);
        }
    }
    void Cache::set_cache(hash_record* cache)
    {
        m_cache=cache;
        m_log->notice("set cache thread hash_map cache");
    }
    void Cache::set_log(LOG_SYSTEM::Log_system* log)
    {
        m_log=log;
        m_log->notice("set cache thread log_system ");
    }
    void Cache::cache_out()
    {
        piter iter=(*m_cache).begin();
        for(iter;iter!=(*m_cache).end();iter++)
        {
        }
    }
}
