#include "../main.h"
#include "game.h"
#include "../util/armhook.h"

void ApplySAMPPatchesInGame();
void InitScripting();

CGame::CGame()
{
	
}

float CGame::GetFPS()
{
	(( void (*)())(g_libGTASA+0x3F5530+1))();
	return (( float (*)())(g_libGTASA+0x3F54C4+1))();
}

void CGame::StartGame()
{
	ApplySAMPPatchesInGame();
	InitScripting();
}

void CGame::Initialize()
{
	
}

void CGame::Process() { }

const char* CGame::GetDataDirectory()
{
	return (const char*)(g_libGTASA+0x6D687C); // StorageRootBuffer
}

bool CGame::IsGamePaused()
{
	return *(bool*)(g_libGTASA+0x96B514) || *(bool*)(g_libGTASA+0x96B515); // CTimer::m_UserPause || CTimer::m_CodePause
}

int CGame::IsLineOfSightClear(float fX, float fY, float fZ)
{
	return 0;
}