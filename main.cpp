#include "main.h"
#include "game/game.h"
#include "util/armhook.h"
#include "log.h"

#include "vendor/rapidjson/document.h"
using namespace rapidjson;

uintptr_t g_libPSA = 0;
uintptr_t g_libGTASA = 0;

CGame *pGame = nullptr;

void InitializeRenderWare();
void InstallGlobalHooks();
void InitScripting();

void *Init(void *p)
{
	CProjectSA::InitPatch();
	InitScripting();
	pthread_exit(0);
}

extern "C" void OnModLoad()
{
	__android_log_print(ANDROID_LOG_DEBUG, "AXLD", "Project SA library loaded from AML! Build time: " __DATE__ " " __TIME__);

	g_libGTASA = ARMHook::getLibraryAddress("libGTASA.so");
	if(g_libGTASA == 0)
	{
		__android_log_print(ANDROID_LOG_DEBUG, "AXLD", "ERROR: libGTASA.so address not found!");
		return;
	}

	ARMHook::makeRET(g_libGTASA+0x3F6580);
	ARMHook::initialiseTrampolines(g_libGTASA+0x3F6584, 0x2D2);

	CProjectSA::InitPatch();
	InstallGlobalHooks();
	InitializeRenderWare();

	pGame = new CGame();

	InitScripting();
	
	Log::Initialize();
}

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
	__android_log_print(ANDROID_LOG_DEBUG, "AXLD", "Project SA library loaded! Build time: " __DATE__ " " __TIME__);

	g_libGTASA = ARMHook::getLibraryAddress("lib_GTA3.so");
	if(g_libGTASA == 0)
	{
		__android_log_print(ANDROID_LOG_DEBUG, "AXLD", "ERROR: libGTASA.so address not found!");
		return 0;
	}

	g_libPSA = ARMHook::getLibraryAddress("libprojectsa.so");
	if (g_libPSA == 0)
	{
		__android_log_print(ANDROID_LOG_DEBUG, "AXLD", "ERROR: libprojectsa.so address not found!");
		return 0;
	}

	ARMHook::makeRET(g_libGTASA+0x3F6580);
	ARMHook::initialiseTrampolines(g_libGTASA+0x3F6584, 0x2D2);

	InstallGlobalHooks();
	InitializeRenderWare();

	pGame = new CGame();

	pthread_t thread;
	pthread_create(&thread, 0, Init, 0);
	
	Log::Initialize();
	return JNI_VERSION_1_4;
}

void JNI_OnUnload(JavaVM *vm, void *reserved)
{
	__android_log_print(ANDROID_LOG_DEBUG, "AXLD", "Project SA library unloaded!");
	ARMHook::uninitializeTrampolines();
}

uint32_t GetTickCount()
{
	struct timeval tv;
	gettimeofday(&tv, nullptr);
	return (tv.tv_sec*1000+tv.tv_usec/1000);
}