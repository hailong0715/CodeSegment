#include <pthread.h>
#include <list>
#include "CTask.h"
typedef unsigned int U32;

class CThreadNotify
{
public:
    CThreadNotify(){};
    ~CThreadNotify(){};
    void Lock(){pthread_mutex_lock(&m_mutex);}
    void UnLock(){pthread_mutex_unlock(&m_mutex);}
    void Wait() { pthread_cond_wait(&m_con,&m_mutex); }
    void Signal() {pthread_cond_signal(&m_con); }
    void Broadcast() { pthread_cond_broadcast(&m_con);}
private:    
    pthread_mutex_t m_mutex;
    pthread_mutexattr_t m_mutex_attr;
    pthread_cond_t m_con;
  
};

class CThread
{
public:
    CThread();
     ~CThread();
    void Start();
     void Run();
    bool AddTask(CTask* pTask);
   static void* StartRoutine(void* arg);
private:
    pthread_t m_thread_id;
    unsigned int m_task_cnt;
    std::list<CTask*> m_task_list; 
    CThreadNotify* m_th_notify;
};
