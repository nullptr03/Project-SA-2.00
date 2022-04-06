#include "main.h"
#include "game/game.h"

void CTaskManager::setMainTask(CTask *pTask)
{
    m_aPrimaryTasks[TASK_PRIMARY_PRIMARY] = pTask;
}

CTask * CTaskManager::getMainTask()
{
    return m_aPrimaryTasks[TASK_PRIMARY_PRIMARY];
}

void CTaskManager::setPedTask(CTask *pTask)
{
    m_aSecondaryTasks[TASK_SECONDARY_IK] = pTask;
}

CTask * CTaskManager::getPedTask()
{
    return m_aSecondaryTasks[TASK_SECONDARY_IK];
}

CTask * CTaskManager::findActiveTaskByType(int taskType)
{
    return (( CTask * (*)(CTaskManager *, int))(g_libGTASA+0x533AB0+1))(this, taskType);
}

CTask * CTaskManager::getActiveTask()
{
    return (( CTask * (*)(CTaskManager *))(g_libGTASA+0x533A32+1))(this);
}