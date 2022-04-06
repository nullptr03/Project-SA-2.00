#pragma once

class CTask;

enum ePrimaryTasks // array indexes
{
    TASK_PRIMARY_PHYSICAL_RESPONSE = 0,
    TASK_PRIMARY_EVENT_RESPONSE_TEMP,
    TASK_PRIMARY_EVENT_RESPONSE_NONTEMP,
    TASK_PRIMARY_PRIMARY,
    TASK_PRIMARY_DEFAULT,
    TASK_PRIMARY_MAX
};

enum eSecondaryTasks // array indexes
{
    TASK_SECONDARY_ATTACK = 0,              // want duck to be after attack
    TASK_SECONDARY_DUCK,                    // because attack controls ducking movement
    TASK_SECONDARY_SAY,
    TASK_SECONDARY_FACIAL_COMPLEX,
    TASK_SECONDARY_PARTIAL_ANIM,
    TASK_SECONDARY_IK,
    TASK_SECONDARY_MAX
};

class CTaskManager {
public:
    CTask* m_aPrimaryTasks[TASK_PRIMARY_MAX];
    CTask* m_aSecondaryTasks[TASK_SECONDARY_MAX];
    PED_TYPE* m_pPed;

public:
    void setMainTask(CTask *pTask);
    CTask* getMainTask();
    void setPedTask(CTask *pTask);
    CTask* getPedTask();
    CTask* findActiveTaskByType(int taskType);
    CTask* getActiveTask();
};