
class CThreadManage
{
private:
    CThreadPool*    m_Pool;
    int          m_NumOfThread;
 
protected:
 
public:
    CThreadManage();
    CThreadManage(int num);
    virtual ~CThreadManage();
 
    void     SetParallelNum(int num);   
    void    Run(CJob* job,void* jobdata);
    void    TerminateAll(void);
};

class CThread
{
private:
    int          m_ErrCode;
    Semaphore    m_ThreadSemaphore;  //the inner semaphore, which is used to realize
    unsigned     long m_ThreadID;  
    bool         m_Detach;       //The thread is detached
    bool         m_CreateSuspended;  //if suspend after creating
    char*        m_ThreadName;
    ThreadState m_ThreadState;      //the state of the thread
 
protected:
    void     SetErrcode(int errcode){m_ErrCode = errcode;}
    static void* ThreadFunction(void*);
 
public:
    CThread();
    CThread(bool createsuspended,bool detach);
    virtual ~CThread();
 
    virtual void Run(void) = 0;
    void     SetThreadState(ThreadState state){m_ThreadState = state;}
  bool     Terminate(void);    //Terminate the threa
    bool     Start(void);        //Start to execute the thread
    void     Exit(void);
    bool     Wakeup(void);
    ThreadState  GetThreadState(void){return m_ThreadState;}
    int      GetLastError(void){return m_ErrCode;}
    void     SetThreadName(char* thrname){strcpy(m_ThreadName,thrname);}
    char*    GetThreadName(void){return m_ThreadName;}
    int      GetThreadID(void){return m_ThreadID;}
    bool     SetPriority(int priority);
    int      GetPriority(void);
    int      GetConcurrency(void);
    void     SetConcurrency(int num);
    bool     Detach(void);
    bool     Join(void);
    bool     Yield(void);
    int      Self(void);
};

class CThreadPool
{
friend class CWorkerThread;
public:
    CThreadPool();
    CThreadPool(int initnum);
    virtual ~CThreadPool();
    void SetMaxNum(int maxnum){m_MaxNum = maxnum;}
    int GetMaxNum(void){return m_MaxNum;}
    void SetAvailLowNum(int minnum){m_AvailLow = minnum;}
    int GetAvailLowNum(void){return m_AvailLow;}
    void SetAvailHighNum(int highnum){m_AvailHigh = highnum;}
    int GetAvailHighNum(void){return m_AvailHigh;}
    int GetActualAvailNum(void){return m_AvailNum;}
    int GetAllNum(void){return m_ThreadList.size();}
    int GetBusyNum(void){return m_BusyList.size();}
    void SetInitNum(int initnum){m_InitNum = initnum;}
    int GetInitNum(void){return m_InitNum;}
    void TerminateAll(void);
    void Run(CJob* job,void* jobdata);
protected:
    CWorkerThread* GetIdleThread(void);
    void AppendToIdleList(CWorkerThread* jobthread);
    void MoveToBusyList(CWorkerThread* idlethread);
    void MoveToIdleList(CWorkerThread* busythread);
    void DeleteIdleThread(int num);
    void CreateIdleThread(int num);
public:
    CThreadMutex m_BusyMutex; //when visit busy list,use m_BusyMutex to lock and unlock
    CThreadMutex m_IdleMutex; //when visit idle list,use m_IdleMutex to lock and unlock
    CThreadMutex m_JobMutex; //when visit job list,use m_JobMutex to lock and unlock
    CThreadMutex m_VarMutex;
    CCondition m_BusyCond; //m_BusyCond is used to sync busy thread list
    CCondition m_IdleCond; //m_IdleCond is used to sync idle thread list
    CCondition m_IdleJobCond; //m_JobCond is used to sync job list
    CCondition m_MaxNumCond;
    vector<CWorkerThread*> m_ThreadList;
    vector<CWorkerThread*> m_BusyList; //Thread List
    vector<CWorkerThread*> m_IdleList; //Idle List
private:
    unsigned int m_InitNum; //Normal thread num;
    unsigned int m_MaxNum; //the max thread num that can create at the same time
    unsigned int m_AvailLow; //The min num of idle thread that shoule kept
    unsigned int m_AvailHigh; //The max num of idle thread that kept at the same time
    unsigned int m_AvailNum; //the normal thread num of idle num;
};

class CWorkerThread:public CThread
{
private:
    CThreadPool* m_ThreadPool;
    CJob* m_Job;
    void* m_JobData;
    CThreadMutex m_VarMutex;
    bool m_IsEnd;
    protected:
    public:
    CCondition m_JobCond;
    CThreadMutex m_WorkMutex;
    CWorkerThread();
    virtual ~CWorkerThread();
    void Run();
    void SetJob(CJob* job,void* jobdata);
    CJob* GetJob(void){return m_Job;}
    void SetThreadPool(CThreadPool* thrpool);
    CThreadPool* GetThreadPool(void){return m_ThreadPool;}
};

class CJob
{
private:
    int      m_JobNo;        //The num was assigned to the job
    char*    m_JobName;      //The job name
    CThread  *m_pWorkThread;     //The thread associated with the job
public:
    CJob( void );
    virtual ~CJob();
    int      GetJobNo(void) const { return m_JobNo; }
    void     SetJobNo(int jobno){ m_JobNo = jobno;}
    char*    GetJobName(void) const { return m_JobName; }
    void     SetJobName(char* jobname);
     CThread *GetWorkThread(void){ return m_pWorkThread; }
    void     SetWorkThread ( CThread *pWorkThread ){
        m_pWorkThread = pWorkThread;
    }
    virtual void Run ( void *ptr ) = 0;
};