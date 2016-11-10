#include "ThreadPool.h"
#include <iostream>
#include <cstdlib>
using namespace std;
ThreadPool::ThreadPool()
{
	m_thread_size = 10;
}

ThreadPool::ThreadPool(unsigned int size)
{
	m_thread_size = size;
}

ThreadPool::~ThreadPool()
{}

int ThreadPool::Init()
{
	m_pWorkThread = new CThread[m_thread_size];
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
	unsigned int index = random()%m_thread_size;
	m_pWorkThread[index].AddTask(pTask);
}