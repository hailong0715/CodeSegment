#include "CTask.h"
#include <iostream>
#include <unistd.h>
using namespace std;
CTask::CTask(unsigned int taskId)
{
	m_task_id = taskId;
}

CTask::~CTask()
{}

void CTask::Run()
{
	cout<<"task:"<<m_task_id<<" do Some Work"<<endl;
	sleep(1);
}