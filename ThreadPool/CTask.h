class CTask
{
public:
    CTask(unsigned int taskId);
    ~CTask();
    void Run();
    unsigned int GetTaskId(){return m_task_id;}

private:
    unsigned int m_task_id; 
};
