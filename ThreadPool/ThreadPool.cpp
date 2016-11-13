#include "CThread.h"
#include <iostream>
#include <cstdlib>

ThreadPool* ThreadPool::m_pThreadPool = NULL;
using namespace std;


ThreadPool::ThreadPool()
{
	m_thread_size = 10;
}

ThreadPool::~ThreadPool()
{}

int ThreadPool::Init(U32 thread_num)
{
	m_thread_size = thread_num;
	m_pWorkThread = new CThread[thread_num];
	if(!m_pWorkThread)
		return 1;
	for(int i=0; i<m_thread_size; i++)
	{
		m_pWorkThread[i].Start();
	}
	return 0;
}

void ThreadPool::Destory()
{
	if(m_pWorkThread)
		delete [] m_pWorkThread;
	m_thread_size = 0;
}

void ThreadPool::AddTask(CTask * pTask)
{
	Lock();
	m_task_list.push_back(pTask);
	UnLock();
	Signal();
}

ThreadPool* ThreadPool::GetInstace()
{
	if(!m_pThreadPool)
		m_pThreadPool = new ThreadPool();
	return m_pThreadPool;
	
}