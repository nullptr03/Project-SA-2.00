#pragma once

#include <vector>
#include <thread>
#include <mutex>

#define MAX_NUM_CORONAS 64

class CProjectSA
{
public:
    static void InitPatch();
    static void InitHooks();
    static void Update();

    static int (*Idle_orig)(void *thiz, bool a2);
    static int Idle_hook(void *thiz, bool a2);

public:
    class FPSFix
    {
    private:
        std::mutex Mutex;
        std::vector<pid_t> Threads;

    public:
        void Init();
        void Routine();
        void PushThread(pid_t tid);
    };

    class Corona
    {
    public:
        static uint32_t dwCoronas;
        static uint32_t dwParticleIDs[MAX_NUM_CORONAS];

    public:
        static void createParticle(float size = 1.0f);
        static void destroyParticle(uint32_t dwID);
        static void destroyAllParticle();
    };

    class Timer
    {
    public:
        static uint16_t m_snTimeInMilliseconds;
    };

    class Clock
    {
    public:
        static void normaliseGameClock();
        static void setGameClock(uint8_t hours, uint8_t minutes, uint8_t day);

    public:
        static uint16_t ms_nMillisecondsPerGameMinute;
        static uint16_t ms_nLastClockTick;
        static uint16_t ms_nGameClockMonths;
        static uint16_t ms_nGameClockDays;
        static uint16_t ms_nGameClockHours;
        static uint16_t ms_nGameClockMinutes;
        static uint16_t ms_nGameClockSeconds;
        static uint8_t CurrentDay;
    };

    class Screen
    {
    public:
        static void Redirect(uintptr_t addr, uintptr_t toaddr);
    };

    class Camera
    {
    public:
        static uintptr_t Scene;
        static float *f3rdPersonCHairMultX;
        static float *f3rdPersonCHairMultY;
    };

    class Hud
    {
    public:
        static void (*DrawCrossHairs_orig)();
        static void DrawCrossHairs_hook();
    };

    class Placeable
    {
    public:
        static uintptr_t gMatrixList;

    public:
        static int (*InitMatrixArray_orig)();
        static int InitMatrixArray_hook();
    };

    class Pools
    {
    public:
        static void (*Initialise_orig)();
        static void Initialise_hook();
    };

    class AND
    {
    public:
        static void (*RunThread_orig)(void *a1);
        static void RunThread_hook(void *a1);
    };
};