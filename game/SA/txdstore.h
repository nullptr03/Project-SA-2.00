#pragma once

class CTxdStore
{
public:
	static void PushCurrentTxd();
	static void PopCurrentTxd();
	static int FindTxdSlot(char const* name);
	static void SetCurrentTxd(int index);
};