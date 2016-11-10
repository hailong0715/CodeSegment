#include "ThreadPool.h"
#include <unistd.h>
int main()
{
	ThreadPool* pTheadPool = new ThreadPool(5);
	pTheadPool->Init();
	for(int i=0; i<20; i++)
	{
		CTask* pTask = new CTask(i);
		pTheadPool->AddTask(pTask);
	}

	while(1)
	{
		sleep(1000);
	}
	//可以设置线程池结束条件，需要结束的时候释放内存
	pTheadPool->Destory();
	delete pTheadPool;
}

