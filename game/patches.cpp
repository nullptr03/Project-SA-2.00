#include <android/log.h>

#include "../main.h"
#include "../game/game.h"
#include "../util/ARMHook.h"
#include "common.h"

#include "vendor/rapidjson/document.h"
#include "vendor/rapidjson/filereadstream.h"
using namespace rapidjson;

uint8_t g_fps = 90;

void ReadPatchSettingFile()
{
	char file[0x7F];
	sprintf(file, "%spsa.json", getGameDataStorage());
	FILE* fp = fopen(file, "r");
 
	char readBuffer[65536];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	
	Document document;
	document.ParseStream(is);
	
	fclose(fp);
	assert(document["i"].IsInt());
	g_fps = document["i"].GetInt();
}

void RedirectScreen(uintptr_t addr, uintptr_t to)
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
		hook[1] = to;
		ARMHook::writeMemory(addr, (uintptr_t)hook, sizeof(hook));
	}
	else
	{
		uint32_t hook[2];
		hook[0] = 0xE51FF004;
		hook[1] = to;
		ARMHook::writeMemory(addr, (uintptr_t)hook, sizeof(hook));
	}
}

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
	//ARMHook::makeNOP(g_libGTASA+0x3F5670, 2);

	// NOP RwCameraEndUpdate from Idle
	ARMHook::makeNOP(g_libGTASA+0x3F6C8C, 2);

	// loading screen size fix
	ARMHook::writeMemory(g_libGTASA+0x43B07B, (uintptr_t)"\xD0", 1); // BEQ
	ARMHook::writeMemory(g_libGTASA+0x43B0C6, (uintptr_t)"\xB6\xEE\x06\x0A", 4); // On BEQ
	ARMHook::writeMemory(g_libGTASA+0x43B07C, (uintptr_t)"\xB5\xEE\x04\x0A\x00\xBF\x00\xBF\x00\xBF", 10); // Otherwise
	RedirectScreen(g_libGTASA+0x43B024+1, g_libGTASA+0x43B05A+1); // Skip checks... Currently fullscreen splashes are for 4:3 screens and 16:9

	// uv anim fix
	ARMHook::unprotect(g_libGTASA+0x6BD1E4);
	*(bool*)(g_libGTASA+0x6BD1E4) = true;

	// Patch for fps at DoGameState
	ARMHook::unprotect(g_libGTASA+0x5E4978);
	ARMHook::unprotect(g_libGTASA+0x5E4990);
	*(uint8_t*)(g_libGTASA+0x5E4978) = g_fps;
	*(uint8_t*)(g_libGTASA+0x5E4990) = g_fps;
}