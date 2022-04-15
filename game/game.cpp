#include "../main.h"
#include "game.h"
#include "../util/armhook.h"

float CGame::GetFPS()
{
	(( void (*)())(g_libGTASA+0x3F5530+1))();
	return (( float (*)())(g_libGTASA+0x3F54C4+1))();
}

bool CGame::IsGamePaused()
{
	return *(bool*)(g_libGTASA+0x96B514) || *(bool*)(g_libGTASA+0x96B515); // CTimer::m_UserPause || CTimer::m_CodePause
}