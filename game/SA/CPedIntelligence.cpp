#include "main.h"
#include "game/game.h"

CTaskManager * CPedIntelligence::getTaskMgr()
{
    return &this->m_TaskMgr;
}

void CPedIntelligence::flushImmediately(bool bSetPrimaryDefaultTask)
{
    if(getTaskMgr()->findActiveTaskByType(704))
        (( void (*)(CPedIntelligence *, bool))(g_libGTASA+0x4C0A44+1))(this, bSetPrimaryDefaultTask);
}