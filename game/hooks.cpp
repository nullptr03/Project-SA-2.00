#include "../main.h"
#include "../util/armhook.h"
#include "RW/RenderWare.h"
#include "game.h"

extern CGame *pGame;

uint16_t nCurrentRenderObject = 0;

void MainLoop();

extern "C" uintptr_t get_lib() 
{
 	return g_libGTASA;
}

void (*Render2dStuff)();
void Render2dStuff_hook()
{
	Render2dStuff();
	MainLoop();
	return;
}

int (*Idle)(void *thiz, bool a2);
int Idle_hook(void *thiz, bool a2)
{
	int result = Idle(thiz, a2);
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

void (*CStream_InitImageList)();
void CStream_InitImageList_hook()
{
	ARMHook::unprotect(g_libGTASA+0x792DA8);
	ARMHook::unprotect(g_libGTASA+0x792DA4);
	ARMHook::unprotect(g_libGTASA+0x792DD4);
	ARMHook::unprotect(g_libGTASA+0x792DD8);
	ARMHook::unprotect(g_libGTASA+0x792E04);
	ARMHook::unprotect(g_libGTASA+0x792E08);
	ARMHook::unprotect(g_libGTASA+0x792E34);
	ARMHook::unprotect(g_libGTASA+0x792E38);
	ARMHook::unprotect(g_libGTASA+0x792E64);
	ARMHook::unprotect(g_libGTASA+0x792E68);
	ARMHook::unprotect(g_libGTASA+0x792E94);
	ARMHook::unprotect(g_libGTASA+0x792E98);
	ARMHook::unprotect(g_libGTASA+0x792EC4);
	ARMHook::unprotect(g_libGTASA+0x792EC8);
	ARMHook::unprotect(g_libGTASA+0x792EF4);
	ARMHook::unprotect(g_libGTASA+0x792D78);

  	*(uint8_t*)(g_libGTASA+0x792DA8) = 0;
  	*(uint32_t*)(g_libGTASA+0x792DA4) = 0;
  	*(uint32_t*)(g_libGTASA+0x792DD4) = 0;
  	*(uint8_t*)(g_libGTASA+0x792DD8) = 0;
  	*(uint32_t*)(g_libGTASA+0x792E04) = 0;
  	*(uint8_t*)(g_libGTASA+0x792E08) = 0;
  	*(uint32_t*)(g_libGTASA+0x792E34) = 0;
  	*(uint8_t*)(g_libGTASA+0x792E38) = 0;
  	*(uint32_t*)(g_libGTASA+0x792E64) = 0;
  	*(uint8_t*)(g_libGTASA+0x792E68) = 0;
  	*(uint32_t*)(g_libGTASA+0x792E94) = 0;
  	*(uint8_t*)(g_libGTASA+0x792E98) = 0;
  	*(uint32_t*)(g_libGTASA+0x792EC4) = 0;
  	*(uint8_t*)(g_libGTASA+0x792EC8) = 0;
  	*(uint32_t*)(g_libGTASA+0x792EF4) = 0;
  	*(uint8_t*)(g_libGTASA+0x792D78) = 0;

	// CStreaming::AddImageToList
	(( uint32_t (*)(char*, uint32_t))(g_libGTASA+0x2CF760+1))("TEXDB\\GTA3.IMG", 1);
	(( uint32_t (*)(char*, uint32_t))(g_libGTASA+0x2CF760+1))("TEXDB\\GTA_INT.IMG", 1);
}

extern "C" bool TaskEnterVehicle(uintptr_t pVehicle, uintptr_t a2)
{
	// CTask::operator new
	uintptr_t pTask = (( uintptr_t (*)())(g_libGTASA+0x4D6A01))(); 

	// CTaskComplexEnterCarAsDriver::CTaskComplexEnterCarAsDriver
  	(( void (*)(uintptr_t, uintptr_t))(g_libGTASA+0x4F6F71))(pTask, pVehicle); 

  	// CTaskManager::SetTask
  	(( void (*)(uintptr_t, uintptr_t, int, int))(g_libGTASA+0x53390B))(a2, pTask, 3, 0); 

	return true;
}

void __attribute__((naked)) TaskEnterVehicle_hook(uint32_t thiz, uint32_t pVehicle)
{
	// 2.0
	__asm__ volatile("push {r1-r11, lr}\n\t"
		"mov r0, r8\n\t"
		"adds r1, r6, #4\n\t"
		"blx TaskEnterVehicle\n\t"
		"pop {r1-r11, lr}\n\t"
		"blx get_lib\n\t"
		"add r0, #0x400000\n\t"
        "add r0, #0xAC00\n\t"
        "add r0, #0x41\n\t"
        "mov pc, r0\n\t");
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

uintptr_t (*CTxdStore_TxdStoreFindCB)(const char *szTexture);
uintptr_t CTxdStore_TxdStoreFindCB_hook(const char *szTexture)
{
	uintptr_t pTexture = 0;
	uintptr_t GTA_INT_DATABASE = (( uintptr_t (*)(const char *))(g_libGTASA+0x1EAC8C+1))("gta_int");
	uintptr_t GTA3_DATABASE = (( uintptr_t (*)(const char *))(g_libGTASA+0x1EAC8C+1))("gta3");
	int iDatabaseRegisterCount = *(int*)(g_libGTASA+0x6BD174);
	if ( iDatabaseRegisterCount )
	{
		uintptr_t *pDatabaseRegisterHandleList = *(uintptr_t**)(g_libGTASA+0x6BD17C);
		if ( iDatabaseRegisterCount < 1 )
		{
		GTA_INT_DB:
			// TextureDatabaseRuntime::Register
			(( void (*)(uintptr_t))(g_libGTASA+0x1E9BC8+1))(GTA_INT_DATABASE);

			// TextureDatabaseRuntime::GetTexture
			pTexture = (( uintptr_t (*)(const char *))(g_libGTASA+0x1E9CE4+1))(szTexture);

			// TextureDatabaseRuntime::Unregister
			(( void (*)(uintptr_t))(g_libGTASA+0x1E9C80+1))(GTA_INT_DATABASE);

			if ( pTexture )
				return pTexture;

			if ( iDatabaseRegisterCount < 1 )
			{
			GTA3_DB:
				// TextureDatabaseRuntime::Register
				(( void (*)(uintptr_t))(g_libGTASA+0x1E9BC8+1))(GTA3_DATABASE);

				// TextureDatabaseRuntime::GetTexture
				pTexture = (( uintptr_t (*)(const char *))(g_libGTASA+0x1E9CE4+1))(szTexture);

				// TextureDatabaseRuntime::Unregister
				(( void (*)(uintptr_t))(g_libGTASA+0x1E9C80+1))(GTA3_DATABASE);

				if ( pTexture )
					return pTexture;
			}
			else
			{
				int iIndex = 0;
				while(pDatabaseRegisterHandleList[iIndex] != GTA3_DATABASE)
				{
					if(++iIndex >= iDatabaseRegisterCount)
						goto GTA3_DB;
				}
			}
		}
		else
		{
			int iIndex = 0;
			while(pDatabaseRegisterHandleList[iIndex] != GTA_INT_DATABASE)
			{
				if(++iIndex >= iDatabaseRegisterCount)
					goto GTA_INT_DB;
			}
		}
	}

	// RwTexDictionaryGetCurrent
	int iParent = (( int (*)())(g_libGTASA+0x1DBA64+1))();
	if ( !iParent )
		return 0;

	while ( true )
	{
		// RwTexDictionaryFindNamedTexture
		pTexture = (( uintptr_t (*)(int, const char *))(g_libGTASA+0x1DB9B0+1))(iParent, szTexture);
		if ( pTexture )
			break;

		// CTxdStore::GetTxdParent
		iParent = (( int (*)(int))(g_libGTASA+0x5D428C+1))(iParent);
		if ( !iParent )
			return 0;
	}
	return pTexture;
}

void InstallSAMPHooks()
{
	ARMHook::installPLTHook(g_libGTASA+0x675DE4, (uintptr_t)AND_TouchEvent_hook, (uintptr_t*)&AND_TouchEvent);
	ARMHook::installPLTHook(g_libGTASA+0x67589C, (uintptr_t)Render2dStuff_hook, (uintptr_t*)&Render2dStuff);
	ARMHook::installPLTHook(g_libGTASA+0x6710C4, (uintptr_t)Idle_hook, (uintptr_t*)&Idle);
	ARMHook::injectCode(g_libGTASA+0x40AC28, (uintptr_t)TaskEnterVehicle_hook, 0);
	ARMHook::installPLTHook(g_libGTASA+0x676034, (uintptr_t)CTxdStore_TxdStoreFindCB_hook, (uintptr_t*)&CTxdStore_TxdStoreFindCB);
}

void InstallGlobalHooks()
{
	ARMHook::installPLTHook(g_libGTASA+0x674C68, (uintptr_t)CStream_InitImageList_hook, (uintptr_t*)&CStream_InitImageList); 
	ARMHook::installPLTHook(g_libGTASA+0x672468, (uintptr_t)CPools_Initialise_hook, (uintptr_t*)&CPools_Initialise);
	ARMHook::installPLTHook(g_libGTASA+0x675554, (uintptr_t)CPlaceable_InitMatrixArray_hook, (uintptr_t*)&CPlaceable_InitMatrixArray);
	InstallSAMPHooks();
}