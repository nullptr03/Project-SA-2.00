#include "main.h"
#include "game/game.h"
#include "util/armhook.h"
#include "log.h"

// AML plugin configuration (only for AML plugins)
#include "addon/amlmod.h"
MYMOD(com.rockstargames.gtasa, Project SA AML version, 1.0, Andy Prasetya)

uintptr_t g_libPSA = 0;
uintptr_t g_libGTASA = 0;

CGame *pGame = nullptr;

void InitializeRenderWare();
void InitScripting();
 
extern "C" void OnModLoad()
{
	__android_log_print(ANDROID_LOG_DEBUG, "AXLD", "Project SA library loaded from AML! Build time: " __DATE__ " " __TIME__);

	g_libGTASA = ARMHook::getLibraryAddress("libGTASA.so");
	if(g_libGTASA == 0)
	{
		g_libGTASA = ARMHook::getLibraryAddress(GTASA_LIBNAME);
		if(g_libGTASA == 0)
		{
			__android_log_print(ANDROID_LOG_ERROR, "AXLD", "Failed to find " GTASA_LIBNAME "!");
			return;
		}
	}

	ARMHook::makeRET(g_libGTASA+0x3F6580);
	ARMHook::initialiseTrampolines(g_libGTASA+0x3F6584, 0x2D2);

	CProjectSA::InitPatch();
	CProjectSA::InitHooks();
	InitializeRenderWare();

	pGame = new CGame();
	InitScripting();
	
	Log::Initialize();
}

extern "C" void OnModPreLoad()
{
	__android_log_print(ANDROID_LOG_DEBUG, "AXLD", "Project SA is not support for preloaded");
}

extern "C" void OnModUnLoad()
{
	__android_log_print(ANDROID_LOG_DEBUG, "AXLD", "Project SA library unloaded!");
	ARMHook::uninitializeTrampolines();
}

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
	__android_log_print(ANDROID_LOG_DEBUG, "AXLD", "Project SA library loaded! Build time: " __DATE__ " " __TIME__);

	g_libGTASA = ARMHook::getLibraryAddress("libGTASA.so");
	if(g_libGTASA == 0)
	{
		g_libGTASA = ARMHook::getLibraryAddress(GTASA_LIBNAME);
		if(g_libGTASA == 0)
		{
			__android_log_print(ANDROID_LOG_ERROR, "AXLD", "Failed to find " GTASA_LIBNAME "!");
			return 0;
		}
	}

	g_libPSA = ARMHook::getLibraryAddress("libprojectsa.so");
	if (g_libPSA == 0)
	{
		__android_log_print(ANDROID_LOG_ERROR, "AXLD", "Failed to find libprojectsa.so!");
		return 0;
	}

	ARMHook::makeRET(g_libGTASA+0x3F6580);
	ARMHook::initialiseTrampolines(g_libGTASA+0x3F6584, 0x2D2);

	CProjectSA::InitPatch();
	CProjectSA::InitHooks();
	InitializeRenderWare();

	pGame = new CGame();
	InitScripting();
	
	Log::Initialize();
	return JNI_VERSION_1_4;
}

void JNI_OnUnload(JavaVM *vm, void *reserved)
{
	__android_log_print(ANDROID_LOG_DEBUG, "AXLD", "Project SA library unloaded!");
	ARMHook::uninitializeTrampolines();
}