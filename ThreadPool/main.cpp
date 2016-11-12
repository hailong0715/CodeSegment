#include "CThread.h"
#include <unistd.h>
int main()
{
	ThreadPool* pTheadPool = ThreadPool::GetInstace();
	pTheadPool->Init(5);
	for(int i=0; i<20; i++)
	{
		CTask* pTask = new CTask(i);
		pTheadPool->AddTask(pTask);
	}

	sleep(10);
	//可以设置线程池结束条件，需要结束的时候释放内存
	pTheadPool->Destory();
	delete pTheadPool;
	return 0;
}

