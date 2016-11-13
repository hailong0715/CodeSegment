#include "CThread.h"
#include <iostream>
using namespace std;

CThreadNotify::CThreadNotify()
{
	pthread_mutex_init(&m_mutex, NULL);
	pthread_cond_init(&m_con, NULL);
}

CThreadNotify::~CThreadNotify()
{
	pthread_mutex_destroy(&m_mutex);
	pthread_cond_destroy(&m_con);
}
CThread::CThread()
{
	m_thread_id = 0;
}

CThread::~CThread()
{

}

void CThread::Start()
{
	(void)pthread_create(&m_thread_id, NULL, StartRoutine, this);
}
void CThread::Run()
{

	for(; ; )
	{
		if(ThreadPool::GetInstace()->IsEmpty())
		{
			cout<<"thread " <<m_thread_id <<" task list is empty\n";
			ThreadPool::GetInstace()->Wait();
		}

		ThreadPool::GetInstace()->Lock();
		std::list<CTask*>& task_list = ThreadPool::GetInstace()->GetTaskList();
		CTask* pTask = task_list.front();
		task_list.pop_front(); 
		ThreadPool::GetInstace()->UnLock();
		cout<<"Thread "<<pthread_self()<<" run Task "<<pTask->GetTaskId()<<endl;
		pTask->Run();
		m_task_cnt--;
		delete pTask;
	}
}

void* CThread::StartRoutine(void * arg)
{
	CThread* pThread = (CThread*)arg;
	pThread->Run();
}

