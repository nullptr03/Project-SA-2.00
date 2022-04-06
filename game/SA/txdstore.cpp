#include "main.h"
#include "game/game.h"

void CTxdStore::PushCurrentTxd()
{
	(( void (*)())(g_libGTASA+0x5D4184+1))();
}

void CTxdStore::PopCurrentTxd()
{
	(( void (*)())(g_libGTASA+0x5D41C4+1))();
}

int CTxdStore::FindTxdSlot(char const* name)
{
	return (( int (*)(char const*))(g_libGTASA+0x5D3E60+1))(name);
}

void CTxdStore::SetCurrentTxd(int index)
{
	(( void (*)(int, uint32_t))(g_libGTASA+0x5D40F4+1))(index, 0);
}