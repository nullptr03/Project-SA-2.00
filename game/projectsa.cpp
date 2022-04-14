#include "main.h"
#include "game.h"
#include "util/armhook.h"
#include <sys/syscall.h>

extern CGame *pGame;

void CProjectSA::InitPatch()
{
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
    // Coronas
    CProjectSA::Corona::dwCoronas = ARMHook::getSymbolAddress("lib_GTA3.so", "_ZN8CCoronas8aCoronasE");

    // Timers
    CProjectSA::Timer::m_snTimeInMilliseconds = (uint16_t)ARMHook::getSymbolAddress("lib_GTA3.so", "_ZN6CTimer22m_snTimeInMillisecondsE");

    // Clocks
    CProjectSA::Clock::ms_nMillisecondsPerGameMinute = (uint16_t)ARMHook::getSymbolAddress("lib_GTA3.so", "_ZN6CClock29ms_nMillisecondsPerGameMinuteE");
    CProjectSA::Clock::ms_nLastClockTick = (uint16_t)ARMHook::getSymbolAddress("lib_GTA3.so", "_ZN6CClock17ms_nLastClockTickE");
    CProjectSA::Clock::ms_nGameClockMonths = (uint16_t)ARMHook::getSymbolAddress("lib_GTA3.so", "_ZN6CClock19ms_nGameClockMonthsE");
    CProjectSA::Clock::ms_nGameClockDays = (uint16_t)ARMHook::getSymbolAddress("lib_GTA3.so", "_ZN6CClock17ms_nGameClockDaysE");
    CProjectSA::Clock::ms_nGameClockHours = (uint16_t)ARMHook::getSymbolAddress("lib_GTA3.so", "_ZN6CClock18ms_nGameClockHoursE");
    CProjectSA::Clock::ms_nGameClockMinutes = (uint16_t)ARMHook::getSymbolAddress("lib_GTA3.so", "_ZN6CClock20ms_nGameClockMinutesE");
    CProjectSA::Clock::ms_nGameClockSeconds = ARMHook::getSymbolAddress("lib_GTA3.so", "_ZN6CClock20ms_nGameClockSecondsE");
    CProjectSA::Clock::CurrentDay = (uint8_t)ARMHook::getSymbolAddress("lib_GTA3.so", "_ZN6CClock10CurrentDayE");
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

void CProjectSA::Corona::createParticle(float size)
{
    for(int i = 0; i < MAX_NUM_CORONAS; i++)
    {
        CRegisteredCorona *aCoronas = (CRegisteredCorona*)((i * 60) + dwCoronas);
        if( aCoronas->m_nFadeState > 0 || 
            aCoronas->m_bRegisteredThisFrame > 0 ||
            aCoronas->m_nFlareType > FLARETYPE_NONE )
        {
            dwParticleIDs[i] = ScriptCommand(&create_particle_at, 3, 0.0f, size, 5.5f, 255, aCoronas->m_nFadeState, aCoronas->m_bRegisteredThisFrame, 
                                            aCoronas->m_nFlareType, aCoronas->m_vPosn.X, aCoronas->m_vPosn.Y, aCoronas->m_vPosn.Z);
        }
    }
}

void CProjectSA::Corona::destroyParticle(uint32_t dwID)
{
    if(dwID)
    {
        ScriptCommand(&destroy_particle, dwID);

        for(int i = 0; i < MAX_NUM_CORONAS; i++)
        {
            if(dwParticleIDs[i] == dwID)
                dwParticleIDs[i] = 0;
        }
    }
}

void CProjectSA::Corona::destroyAllParticle()
{
    for(int i = 0; i < MAX_NUM_CORONAS; i++)
    {
        if(dwParticleIDs[i])
        {
            ScriptCommand(&destroy_particle, dwParticleIDs[i]);
            dwParticleIDs[i] = 0;
        }
    }
}

void CProjectSA::Clock::normaliseGameClock()
{
    (( void (*)())(g_libGTASA+0x3E31C0+1))();
}

void CProjectSA::Clock::setGameClock(uint8_t hours, uint8_t minutes, uint8_t day)
{
    (( void (*)(uint8_t, uint8_t, uint8_t))(g_libGTASA+0x3E3070+1))(hours, minutes, day);
}

// Coronas
uint32_t CProjectSA::Corona::dwCoronas;
uint32_t CProjectSA::Corona::dwParticleIDs[MAX_NUM_CORONAS];

// Timers
uint16_t CProjectSA::Timer::m_snTimeInMilliseconds;

// Clocks
uint16_t CProjectSA::Clock::ms_nMillisecondsPerGameMinute;
uint16_t CProjectSA::Clock::ms_nLastClockTick;
uint16_t CProjectSA::Clock::ms_nGameClockMonths;
uint16_t CProjectSA::Clock::ms_nGameClockDays;
uint16_t CProjectSA::Clock::ms_nGameClockHours;
uint16_t CProjectSA::Clock::ms_nGameClockMinutes;
uint16_t CProjectSA::Clock::ms_nGameClockSeconds;
uint8_t CProjectSA::Clock::CurrentDay;