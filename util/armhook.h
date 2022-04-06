#pragma once

class ARMHook
{
private:
	static uintptr_t local_trampoline;
	static uintptr_t remote_trampoline;
	
public:
	static uintptr_t getLibraryAddress(const char* library);
	static uint8_t getByteSumFromAddress(uintptr_t dest, uint16_t count);
	static uintptr_t getSymbolAddress(const char *library, const char *symbol);
	static void initialiseTrampolines(uintptr_t dest, uintptr_t size);
	static void uninitializeTrampolines();
	static void unprotect(uintptr_t ptr);
	static void writeMemory(uintptr_t dest, uintptr_t src, size_t size);
	static void readMemory(uintptr_t dest, uintptr_t src, size_t size);
	static void makeRET(uintptr_t dest);
	static void makeNOP(uintptr_t addr, unsigned int count);
	static void makeJump(uintptr_t func, uintptr_t addr);
	static void writeMemHookProc(uintptr_t addr, uintptr_t func);
	static void installPLTHook(uintptr_t addr, uintptr_t func, uintptr_t *orig);
	static void installHook(uintptr_t addr, uintptr_t func, uintptr_t *orig);
	static void installMethodHook(uintptr_t addr, uintptr_t func);
	static void putCode(uintptr_t addr, uintptr_t point, uintptr_t func);
	static void injectCode(uintptr_t addr, uintptr_t func, int reg);
};