#include <android/log.h>

#include "../main.h"
#include "../util/ARMHook.h"
#include "common.h"

uint8_t g_fps = 90;

void ApplyGlobalPatches()
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
	ARMHook::makeNOP(g_libGTASA+0x3F5670, 2);

	// NOP RwCameraEndUpdate from Idle
	ARMHook::makeNOP(g_libGTASA+0x3F6C8C, 2);
}

void ApplySAMPPatchesInGame()
{
	// Patch for fps at DoGameState
	ARMHook::unprotect(g_libGTASA+0x5E4978);
	ARMHook::unprotect(g_libGTASA+0x5E4990);
	*(uint8_t*)(g_libGTASA+0x5E4978) = g_fps;
	*(uint8_t*)(g_libGTASA+0x5E4990) = g_fps;
}