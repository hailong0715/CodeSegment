#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_
#include "CThread.h"
class ThreadPool
{
public:

   virtual ~ThreadPool();
    int Init(U32 thread_num);
    void Destory();
    void AddTask(CTask* pTask);
    static ThreadPool* GetInstace();
        
private:
    ThreadPool();
    static ThreadPool* m_pThreadPool;
    unsigned int m_thread_size;
    CThread* m_pWorkThread;
};
#endif
