#include "../main.h"
#include "../util/armhook.h"
#include "RW/RenderWare.h"
#include "game.h"

extern CGame *pGame;
CProjectSA::FPSFix *g_fpsFix = new CProjectSA::FPSFix();

uint16_t nCurrentRenderObject = 0;

extern "C" uintptr_t get_lib() 
{
 	return g_libGTASA;
}

int (*Idle)(void *thiz, bool a2);
int Idle_hook(void *thiz, bool a2)
{
	int result = Idle(thiz, a2);
	CProjectSA::Update();
	RwCameraEndUpdate(*(RwCamera**)(g_libGTASA+0x9FC93C));
	return result;
}

void (*AND_TouchEvent)(int, int, int, int);
void AND_TouchEvent_hook(int type, int num, int x, int y)
{
	if(pGame->IsGamePaused())
		return AND_TouchEvent(type, num, x, y);
	
	return AND_TouchEvent(type, num, x, y);
}

void (*CPools_Initialise)(void);
void CPools_Initialise_hook(void)
{
	struct PoolAllocator
	{
		struct Pool
		{
			void *objects;
			uint8_t *flags;
			uint32_t count;
			uint32_t top;
			uint32_t bInitialized;
		};
		static_assert(sizeof(Pool) == 0x14);

		static Pool* Allocate(size_t count, size_t size)
		{
			Pool *p = new Pool;

			p->objects = new char[size*count];
			p->flags = new uint8_t[count];
			p->count = count;
			p->top = 0xFFFFFFFF;
			p->bInitialized = 1;

			for (size_t i = 0; i < count; i++)
			{
				p->flags[i] |= 0x80;
				p->flags[i] &= 0x80;
			}
			return p;
		}
	};
	
	static auto ms_pPtrNodeSingleLinkPool	= 	PoolAllocator::Allocate(100000, 8);
	static auto ms_pPtrNodeDoubleLinkPool 	= 	PoolAllocator::Allocate(20000, 12);
	static auto ms_pEntryInfoNodePool 		= 	PoolAllocator::Allocate(20000, 20);
	static auto ms_pPedPool					= 	PoolAllocator::Allocate(240, 1996);
	static auto ms_pVehiclePool 			=	PoolAllocator::Allocate(2000, 2604);
	static auto ms_pBuildingPool 			=	PoolAllocator::Allocate(20000, 60);
	static auto ms_pObjectPool 				= 	PoolAllocator::Allocate(3000, 420);
	static auto ms_pDummyPool 				= 	PoolAllocator::Allocate(40000, 60);
	static auto ms_pColModelPool 			= 	PoolAllocator::Allocate(50000, 48);
	static auto ms_pTaskPool 				= 	PoolAllocator::Allocate(5000, 128);
	static auto ms_pEventPool 				= 	PoolAllocator::Allocate(1000, 68);
	static auto ms_pPointRoutePool 			= 	PoolAllocator::Allocate(200, 100);
	static auto ms_pPatrolRoutePool 		= 	PoolAllocator::Allocate(200, 420);
	static auto ms_pNodeRoutePool 			= 	PoolAllocator::Allocate(200, 36);
	static auto ms_pTaskAllocatorPool		= 	PoolAllocator::Allocate(3000, 32);
	static auto ms_pPedIntelligencePool 	= 	PoolAllocator::Allocate(240, 664);
	static auto ms_pPedAttractorPool 		= 	PoolAllocator::Allocate(200, 236);

	uintptr_t g_pPool = g_libGTASA + 0x95AC38;
	*(PoolAllocator::Pool**)(g_pPool) = ms_pPtrNodeSingleLinkPool;
	*(PoolAllocator::Pool**)(g_pPool + 4) = ms_pPtrNodeDoubleLinkPool;
	*(PoolAllocator::Pool**)(g_pPool + 8) = ms_pEntryInfoNodePool;
	*(PoolAllocator::Pool**)(g_pPool + 12) = ms_pPedPool;
	*(PoolAllocator::Pool**)(g_pPool + 16) = ms_pVehiclePool;
	*(PoolAllocator::Pool**)(g_pPool + 20) = ms_pBuildingPool;
	*(PoolAllocator::Pool**)(g_pPool + 24) = ms_pObjectPool;
	*(PoolAllocator::Pool**)(g_pPool + 28) = ms_pDummyPool;
	*(PoolAllocator::Pool**)(g_pPool + 32) = ms_pColModelPool;
	*(PoolAllocator::Pool**)(g_pPool + 36) = ms_pTaskPool;
	*(PoolAllocator::Pool**)(g_pPool + 40) = ms_pEventPool;
	*(PoolAllocator::Pool**)(g_pPool + 44) = ms_pPointRoutePool;
	*(PoolAllocator::Pool**)(g_pPool + 48) = ms_pPatrolRoutePool;
	*(PoolAllocator::Pool**)(g_pPool + 52) = ms_pNodeRoutePool;
	*(PoolAllocator::Pool**)(g_pPool + 56) = ms_pTaskAllocatorPool;
	*(PoolAllocator::Pool**)(g_pPool + 60) = ms_pPedIntelligencePool;
	*(PoolAllocator::Pool**)(g_pPool + 64) = ms_pPedAttractorPool;
	cacheflush(g_pPool, g_pPool + 64, 0);
}

int (*CPlaceable_InitMatrixArray)();
int CPlaceable_InitMatrixArray_hook()
{
	return (( int (*)(uintptr_t, int))(g_libGTASA+0x407F84+1))(g_libGTASA+0x95A988, 10000);
}

void (*ANDRunThread)(void *a1);
void ANDRunThread_hook(void *a1)
{
	g_fpsFix->PushThread(gettid());
	ANDRunThread(a1);
}

void InstallGlobalHooks()
{
	g_fpsFix->Init();
	
	ARMHook::installPLTHook(g_libGTASA+0x6710C4, (uintptr_t)Idle_hook, (uintptr_t*)&Idle);
	ARMHook::installHook(g_libGTASA+0x26BF20, (uintptr_t)ANDRunThread_hook, (uintptr_t*)&ANDRunThread);
	ARMHook::installPLTHook(g_libGTASA+0x675DE4, (uintptr_t)AND_TouchEvent_hook, (uintptr_t*)&AND_TouchEvent);

	ARMHook::installPLTHook(g_libGTASA+0x672468, (uintptr_t)CPools_Initialise_hook, (uintptr_t*)&CPools_Initialise);
	ARMHook::installPLTHook(g_libGTASA+0x675554, (uintptr_t)CPlaceable_InitMatrixArray_hook, (uintptr_t*)&CPlaceable_InitMatrixArray);
}