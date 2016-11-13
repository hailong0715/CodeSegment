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
	//可以设置线程池结束条件，需要结束的时候释放内存
	pTheadPool->Destory();
	delete pTheadPool;
	return 0;
}

