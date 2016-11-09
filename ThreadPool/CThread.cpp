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
	
		if(m_task_list.empty())
		{
			cout<<"task list is empty"<<endl;
			m_th_notify->Wait();
		}
		m_th_notify->Lock();
		CTask* pTask = m_task_list.front();
		pTask->Run();
		m_th_notify->UnLock();
		m_task_cnt--;
		cout<<"Thread "<<m_thread_id<<" run Task "<<pTask->GetTaskId()<<endl;
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
	m_task_list.push_back(pTask);
	m_th_notify->Signal();
	m_th_notify->UnLock();
	cout<<"Push Task taskId:"<<pTask->GetTaskId()<<" into thread threadId:"<<m_thread_id<<endl;
}