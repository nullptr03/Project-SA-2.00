#include "main.h"
#include "game/game.h"
#include "util/armhook.h"
#include <sys/syscall.h>

extern CGame *pGame;
CProjectSA::FPSFix *g_fpsFix = new CProjectSA::FPSFix();

void CProjectSA::InitPatch()
{
	// Placeable
	CProjectSA::Placeable::gMatrixList = ARMHook::getSymbolAddress(GTASA_LIBNAME, "gMatrixList");

	// Timers
	CProjectSA::Timer::m_snTimeInMilliseconds = (uint16_t*)ARMHook::getSymbolAddress(GTASA_LIBNAME, "_ZN6CTimer22m_snTimeInMillisecondsE");

	// Clocks
	CProjectSA::Clock::ms_nMillisecondsPerGameMinute = (uint16_t*)ARMHook::getSymbolAddress(GTASA_LIBNAME, "_ZN6CClock29ms_nMillisecondsPerGameMinuteE");
	CProjectSA::Clock::ms_nLastClockTick = (uint16_t*)ARMHook::getSymbolAddress(GTASA_LIBNAME, "_ZN6CClock17ms_nLastClockTickE");
	CProjectSA::Clock::ms_nGameClockMonths = (uint16_t*)ARMHook::getSymbolAddress(GTASA_LIBNAME, "_ZN6CClock19ms_nGameClockMonthsE");
	CProjectSA::Clock::ms_nGameClockDays = (uint16_t*)ARMHook::getSymbolAddress(GTASA_LIBNAME, "_ZN6CClock17ms_nGameClockDaysE");
	CProjectSA::Clock::ms_nGameClockHours = (uint16_t*)ARMHook::getSymbolAddress(GTASA_LIBNAME, "_ZN6CClock18ms_nGameClockHoursE");
	CProjectSA::Clock::ms_nGameClockMinutes = (uint16_t*)ARMHook::getSymbolAddress(GTASA_LIBNAME, "_ZN6CClock20ms_nGameClockMinutesE");
	CProjectSA::Clock::ms_nGameClockSeconds = (uint16_t*)ARMHook::getSymbolAddress(GTASA_LIBNAME, "_ZN6CClock20ms_nGameClockSecondsE");
	CProjectSA::Clock::CurrentDay = (uint8_t*)ARMHook::getSymbolAddress(GTASA_LIBNAME, "_ZN6CClock10CurrentDayE");

	// Camera
	CProjectSA::Camera::Scene = ARMHook::getSymbolAddress(GTASA_LIBNAME, "Scene");
	CProjectSA::Camera::f3rdPersonCHairMultX = (float*)ARMHook::getSymbolAddress(GTASA_LIBNAME, "_ZN7CCamera22m_f3rdPersonCHairMultXE");
	CProjectSA::Camera::f3rdPersonCHairMultY = (float*)ARMHook::getSymbolAddress(GTASA_LIBNAME, "_ZN7CCamera22m_f3rdPersonCHairMultYE");

	// _rwOpenGLRasterCreate
	ARMHook::writeMemory(g_libGTASA+0x1AE95E, (uintptr_t)"\x01\x22", 2);

	// CVehicleModelInfo::SetupCommonData ~ Increase matrix
	ARMHook::writeMemory(g_libGTASA+0x468B7E, (uintptr_t)"\x4F\xF4\x00\x30", 4);
	ARMHook::writeMemory(g_libGTASA+0x468B88, (uintptr_t)"\xF7\x20", 2);
	ARMHook::writeMemory(g_libGTASA+0x468B8A, (uintptr_t)"\xF7\x25", 2);
	ARMHook::writeMemory(g_libGTASA+0x468BCC, (uintptr_t)"\xF7\x28", 2);

	// RpWorldAddLight from ~LightsCreate
	ARMHook::makeNOP(g_libGTASA+0x46FC54, 2);

	// CFont::PrintString from DisplayFPS
	//ARMHook::makeNOP(g_libGTASA+0x3F5670, 2);

	// NOP RwCameraEndUpdate from Idle
	ARMHook::makeNOP(g_libGTASA+0x3F6C8C, 2);

	// loading screen size fix
	ARMHook::writeMemory(g_libGTASA+0x43B07B, (uintptr_t)"\xD0", 1); // BEQ
	ARMHook::writeMemory(g_libGTASA+0x43B0C6, (uintptr_t)"\xB6\xEE\x06\x0A", 4); // On BEQ
	ARMHook::writeMemory(g_libGTASA+0x43B07C, (uintptr_t)"\xB5\xEE\x04\x0A\x00\xBF\x00\xBF\x00\xBF", 10); // Otherwise
	CProjectSA::Screen::Redirect(g_libGTASA+0x43B024+1, g_libGTASA+0x43B05A+1); // Skip checks... Currently fullscreen splashes are for 4:3 screens and 16:9

	// uv anim fix
	ARMHook::unprotect(g_libGTASA+0x6BD1E4);
	*(bool*)(g_libGTASA+0x6BD1E4) = true;
	
	// BrightLight Render (Trafficlight Render for android)
	ARMHook::writeMemory(g_libGTASA+0x3F63EA, (uintptr_t)"\xCC\xF1\x43\xF8", 4);

	// Cloud Render
	ARMHook::writeMemory(g_libGTASA+0x3F637C, (uintptr_t)"\xAA\xF1\x0C\xFD", 4);
	ARMHook::writeMemory(g_libGTASA+0x3F6380, (uintptr_t)"\xAB\xF1\x22\xFC", 4);

	// realtime game time
	*(uint16_t*)(g_libGTASA+0x953138) = 60 * 1000;

	// fast render
	float fRenderC = 1.75f;
	ARMHook::unprotect(g_libGTASA+0x1E97C8);
	ARMHook::unprotect(g_libGTASA+0x1E97C4);
	ARMHook::unprotect(g_libGTASA+0x1E97BC);
	*(float*)(g_libGTASA+0x1E97C8) = (225280.0 * fRenderC);
	*(float*)(g_libGTASA+0x1E97C4) = (512000.0 * fRenderC);
	*(float*)(g_libGTASA+0x1E97BC) = (768000.0 * fRenderC);

	// Patch for fps at DoGameState
	ARMHook::unprotect(g_libGTASA+0x5E4978);
	ARMHook::unprotect(g_libGTASA+0x5E4990);
	*(uint8_t*)(g_libGTASA+0x5E4978) = 90;
	*(uint8_t*)(g_libGTASA+0x5E4990) = 90;
}

void CProjectSA::Update()
{
	// Camera
	CProjectSA::Camera::Scene = ARMHook::getSymbolAddress(GTASA_LIBNAME, "Scene");
}

void CProjectSA::Screen::Redirect(uintptr_t addr, uintptr_t toaddr)
{
	if(!addr) return;
	if(addr & 1)
	{
		addr &= ~1;
		if (addr & 2)
		{
			ARMHook::makeNOP(addr, 1);
			addr += 2;
		}
		uint32_t hook[2];
		hook[0] = 0xF000F8DF;
		hook[1] = toaddr;
		ARMHook::writeMemory(addr, (uintptr_t)hook, sizeof(hook));
	}
	else
	{
		uint32_t hook[2];
		hook[0] = 0xE51FF004;
		hook[1] = toaddr;
		ARMHook::writeMemory(addr, (uintptr_t)hook, sizeof(hook));
	}
}

void CProjectSA::FPSFix::Init()
{
	std::thread(&CProjectSA::FPSFix::Routine, this).detach();
}

void CProjectSA::FPSFix::Routine()
{
	while (true)
	{
		Mutex.lock();
		for (auto& i : Threads)
		{
			uint32_t mask = 0xff;
			syscall(__NR_sched_setaffinity, i, sizeof(mask), &mask);
		}
		Mutex.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

void CProjectSA::FPSFix::PushThread(pid_t tid)
{
	std::lock_guard<std::mutex> lock(Mutex);
	Threads.push_back(tid);
}

void CProjectSA::Clock::NormaliseGameClock()
{
    if (*ms_nGameClockSeconds >= 60) {
        auto leftMins = *ms_nGameClockSeconds / 60;

        // add leftover mins from second counter
        *ms_nGameClockMinutes += leftMins;

        // normalize second counter
        *ms_nGameClockSeconds %= 60;
    }

    if (*ms_nGameClockMinutes >= 60) {
        auto leftHours = *ms_nGameClockMinutes / 60;

        // add leftover mins from minute counter
        *ms_nGameClockHours += leftHours;

        // normalize minute counter
        *ms_nGameClockMinutes %= 60;
    }

    if (*ms_nGameClockHours >= 24) {
        auto leftDays = *ms_nGameClockHours / 24;

        // add leftover days from hour counter
        *ms_nGameClockDays += leftDays;

        // normalize hour counter
        *ms_nGameClockHours %= 24;
    }

    if (*ms_nGameClockDays > 31) {
        // new month
        *ms_nGameClockDays = 1;

        *ms_nGameClockMonths++;
    }

    if (*ms_nGameClockMonths > 12)
        *ms_nGameClockMonths = 1;
}

void CProjectSA::Clock::SetGameClock(uint8_t hours, uint8_t minutes, uint8_t day)
{
    *ms_nLastClockTick = *CProjectSA::Timer::m_snTimeInMilliseconds;
    *ms_nGameClockHours = hours;
    *ms_nGameClockMinutes = minutes;

    if (day) {
        *CurrentDay = day;
        ++*ms_nGameClockDays;
    }

    *ms_nGameClockSeconds = 0;
    NormaliseGameClock();
}

// hooks
void (*CProjectSA::Hud::DrawCrossHairs_orig)();
void CProjectSA::Hud::DrawCrossHairs_hook()
{
	float save1 = *CProjectSA::Camera::f3rdPersonCHairMultX;
	float save2 = *CProjectSA::Camera::f3rdPersonCHairMultY;

	*CProjectSA::Camera::f3rdPersonCHairMultX = 0.52f;
	*CProjectSA::Camera::f3rdPersonCHairMultY = 0.435f;

	CProjectSA::Hud::DrawCrossHairs_orig();

	*CProjectSA::Camera::f3rdPersonCHairMultX = save1;
	*CProjectSA::Camera::f3rdPersonCHairMultY = save2;
}

int (*CProjectSA::Placeable::InitMatrixArray_orig)();
int CProjectSA::Placeable::InitMatrixArray_hook()
{
	return (( int (*)(uintptr_t, int))(g_libGTASA+0x407F84+1))(CProjectSA::Placeable::gMatrixList, 10000);
}

void (*CProjectSA::AND::RunThread_orig)(void *a1);
void CProjectSA::AND::RunThread_hook(void *a1)
{
	g_fpsFix->PushThread(gettid());
	CProjectSA::AND::RunThread_orig(a1);
}

int (*CProjectSA::Idle_orig)(void *thiz, bool a2);
int CProjectSA::Idle_hook(void *thiz, bool a2)
{
	int result = CProjectSA::Idle_orig(thiz, a2);
	CProjectSA::Update();
	RwCameraEndUpdate(*(RwCamera**)(CProjectSA::Camera::Scene+0x4));
	return result;
}

void (*CProjectSA::Pools::Initialise_orig)();
void CProjectSA::Pools::Initialise_hook()
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

void CProjectSA::InitHooks()
{
	g_fpsFix->Init();

	ARMHook::installPLTHook(g_libGTASA+0x6710C4, (uintptr_t)CProjectSA::Idle_hook, (uintptr_t*)&CProjectSA::Idle_orig);
	ARMHook::installHook(g_libGTASA+0x26BF20, (uintptr_t)CProjectSA::AND::RunThread_hook, (uintptr_t*)&CProjectSA::AND::RunThread_orig);
	ARMHook::installPLTHook(g_libGTASA+0x675554, (uintptr_t)CProjectSA::Placeable::InitMatrixArray_hook, (uintptr_t*)&CProjectSA::Placeable::InitMatrixArray_orig);
	ARMHook::installPLTHook(g_libGTASA+0x672468, (uintptr_t)CProjectSA::Pools::Initialise_hook, (uintptr_t*)&CProjectSA::Pools::Initialise_orig);
	ARMHook::installPLTHook(g_libGTASA+0x672880, (uintptr_t)CProjectSA::Hud::DrawCrossHairs_hook, (uintptr_t*)&CProjectSA::Hud::DrawCrossHairs_orig);
}

// Timers
uint16_t *CProjectSA::Timer::m_snTimeInMilliseconds;

// Clocks
uint16_t *CProjectSA::Clock::ms_nMillisecondsPerGameMinute;
uint16_t *CProjectSA::Clock::ms_nLastClockTick;
uint16_t *CProjectSA::Clock::ms_nGameClockMonths;
uint16_t *CProjectSA::Clock::ms_nGameClockDays;
uint16_t *CProjectSA::Clock::ms_nGameClockHours;
uint16_t *CProjectSA::Clock::ms_nGameClockMinutes;
uint16_t *CProjectSA::Clock::ms_nGameClockSeconds;
uint8_t *CProjectSA::Clock::CurrentDay;

// Placeable
uintptr_t CProjectSA::Placeable::gMatrixList;

// Camera
uintptr_t CProjectSA::Camera::Scene;
float* CProjectSA::Camera::f3rdPersonCHairMultX;
float* CProjectSA::Camera::f3rdPersonCHairMultY;