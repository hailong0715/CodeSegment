#include "CThread.h"
#include <unistd.h>
#include <iostream>
using namespace std;
int main()
{
	ThreadPool* pTheadPool = ThreadPool::GetInstace();
	pTheadPool->Init(5);

	cout<<"main";
	for(int i=0; i<100; i++)
	{
		CTask* pTask = new CTask(i);
		pTheadPool->AddTask(pTask);
	}
	cout<<"Task added\n";	

	sleep(30);
	//���������̳߳ؽ�����������Ҫ������ʱ���ͷ��ڴ�
	pTheadPool->Destory();
	delete pTheadPool;
	return 0;
}

