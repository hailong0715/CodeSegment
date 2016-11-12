#include "CThread.h"
#include <iostream>
using namespace std;
CThread::CThread()
{
	m_thread_id = 0;
	m_th_notify = new CThreadNotify();
}

CThread::~CThread()
{
	if(m_th_notify)
		delete m_th_notify;
}

void CThread::Start()
{
	(void)pthread_create(&m_thread_id, NULL, StartRoutine, this);
}
void CThread::Run()
{
	for(; ; )
	{
		m_th_notify->Lock();
		if(ThreadPool::GetInstace()->IsEmpty())
		{
			cout<<"thread " <<m_thread_id <<" task list is empty\n";
			m_th_notify->Wait();
		}
		
		CTask* pTask = ThreadPool::GetInstace()->m_task_list.front();
		cout<<"Thread "<<m_thread_id<<" run Task "<<pTask->GetTaskId()<<endl;
		pTask->Run();
		ThreadPool::GetInstace()->m_task_list.pop_front(); //任务处理完成后退出队列
		m_task_cnt--;
		m_th_notify->UnLock();
		delete pTask;
	}
}

void* CThread::StartRoutine(void * arg)
{
	CThread* pThread = (CThread*)arg;
	pThread->Run();
}

bool CThread::AddTask(CTask* pTask)
{
	m_th_notify->Lock();
	ThreadPool::GetInstace()->AddTask(pTask);
	m_th_notify->UnLock();
	return true;
}
