#include "main.h"
#include "game/game.h"

uintptr_t CTask::getVmtPtr()
{
	uintptr_t task_vtable = *((uintptr_t*)this);
	return task_vtable - g_libGTASA;
}

void CTask::destroyTask()
{
	uintptr_t task_vtable = *((uintptr_t*)this);
	return (( void (*)(CTask *))(*(void**)(task_vtable + 4)))(this);
}

CTask * CTask::getSubTask()
{
	uintptr_t task_vtable = *((uintptr_t*)this);
	return (( CTask * (*)(CTask *))(*(void**)(task_vtable + 12)))(this);
}

int CTask::getTaskType()
{
	uintptr_t task_vtable = *((uintptr_t*)this);
	return (( int (*)(CTask *))(*(void**)(task_vtable + 20)))(this);
}