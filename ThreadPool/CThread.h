#ifndef CTHREAD_H_
#define CTHREAD_H_
#include <pthread.h>
#include "CTask.h"
#include <list>
typedef unsigned int U32;

class CThreadNotify
{
public:
    CThreadNotify();
    ~CThreadNotify();
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
   static void* StartRoutine(void* arg);
private:
    pthread_t m_thread_id;
    unsigned int m_task_cnt;
};

class ThreadPool
{
public:

   virtual ~ThreadPool();
    int Init(U32 thread_num);
    void Destory();
    void AddTask(CTask* pTask);
    void Lock() {m_thread_notify.Lock(); }
    void UnLock() { m_thread_notify.UnLock();}
    void Signal() { m_thread_notify.Signal(); }
    void BroadCast(){m_thread_notify.Broadcast();}
    void Wait() {m_thread_notify.Wait(); }
    std::list<CTask*>& GetTaskList(){ return m_task_list;}
    static ThreadPool* GetInstace();
    bool IsEmpty()
    {
        Lock();
        bool ret = m_task_list.empty();
        UnLock();
        return ret;
        
    } 
    
private:
    ThreadPool();
    CThreadNotify m_thread_notify;
    static ThreadPool* m_pThreadPool;
    U32 m_thread_size;
    CThread* m_pWorkThread;
    std::list<CTask*> m_task_list;  
};

#endif
