#include "CThread.h"

class ThreadPool
{
public:
    ThreadPool();
    ThreadPool(unsigned int size);
    ~ThreadPool();
    int Init();
    void Destory();
    void AddTask(CTask* pTask);
        
private:
    unsigned int m_thread_size;
    CThread* m_pWorkThread;
};
