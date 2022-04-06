#pragma once

class CTask {
public:
    CTask* m_pParentTask;

public:
    uintptr_t getVmtPtr();
    void createTask();
    void destroyTask();
    CTask* getSubTask();
    int getTaskType();
};