#pragma once

class CTaskManager;

class CPedIntelligence {
public:
    uintptr_t m_pPed;
    CTaskManager   m_TaskMgr;

public:
    CTaskManager *getTaskMgr();
    void flushImmediately(bool bSetPrimaryDefaultTask);
};