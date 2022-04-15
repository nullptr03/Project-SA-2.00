#include "main.h"
#include "log.h"
#include "game/game.h"
#include "util/armhook.h"

extern CGame *pGame;

extern uint16_t nCurrentRenderObject;
extern uint32_t dwScmOpcodeDebug;
extern uint8_t bScmLocalDebug;

uintptr_t android_sdk()
{
	return ARMHook::getLibraryAddress("libc.so");
}

void PrintAddressInfo(const char *crashId, void* pc, void* lr)
{
	Dl_info info_pc, info_lr;

	if(dladdr(pc, &info_pc))
		Log::SendCrashLog(crashId, "PC: %s", info_pc.dli_sname);

	if(dladdr(lr, &info_lr))
		Log::SendCrashLog(crashId, "LR: %s", info_lr.dli_sname);
}

void Log::Initialize()
{
	Log::_session_start_tick = rand() % 10000;
	Log::InstallExceptionHandler();
}

void Log::SendCrashLog(const char *crashId, const char *fmt, ...)
{
	char buffer[0xFF];
	static FILE* flLog = nullptr;

	if(!pGame) return;
	if(flLog == nullptr)
	{
		sprintf(buffer, "%sSAMP/crash_%s.log", getGameDataStorage(), crashId);
		flLog = fopen(buffer, "w");
	}

	memset(buffer, 0, sizeof(buffer));

	va_list arg;
	va_start(arg, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, arg);
	va_end(arg);

	__android_log_print(ANDROID_LOG_DEBUG, crashId, "%s", buffer);

	if(flLog == nullptr) return;
	fprintf(flLog, "%s\n", buffer);
	fflush(flLog);
}

void Log::InstallExceptionHandler()
{
	struct sigaction act;
	act.sa_sigaction = Log::handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGSEGV, &act, &Log::act_old);

	struct sigaction act1;
	act1.sa_sigaction = Log::handler1;
	sigemptyset(&act1.sa_mask);
	act1.sa_flags = SA_SIGINFO;
	sigaction(SIGABRT, &act1, &Log::act1_old);

	struct sigaction act2;
	act2.sa_sigaction = Log::handler2;
	sigemptyset(&act2.sa_mask);
	act2.sa_flags = SA_SIGINFO;
	sigaction(SIGFPE, &act2, &Log::act2_old);

	struct sigaction act3;
	act3.sa_sigaction = Log::handler3;
	sigemptyset(&act3.sa_mask);
	act3.sa_flags = SA_SIGINFO;
	sigaction(SIGBUS, &act3, &Log::act3_old);
}

void Log::handler(int signum, siginfo_t *info, void* contextPtr)
{
	ucontext* context = (ucontext_t*)contextPtr;

	if (Log::act_old.sa_sigaction)
		Log::act_old.sa_sigaction(signum, info, contextPtr);

	Log::SendCrashLog("segv", "======= Project SA CRASH LOG =======");
	
	Log::SendCrashLog("segv", "Exception At Address: 0x%X", info->si_addr);
	
	Log::SendCrashLog("segv", "SCM Op: 0x%X, lDbg: %d", dwScmOpcodeDebug, bScmLocalDebug);

	Log::SendCrashLog("segv", "libGTASA.so base address: 0x%X", g_libGTASA);
	Log::SendCrashLog("segv", "libprojectsa.so base address: 0x%X", g_libPSA);

	Log::SendCrashLog("segv", "Register states:");
	Log::SendCrashLog("segv", "r0: 0x%X, r1: 0x%X, r2: 0x%X, r3: 0x%X",
		context->uc_mcontext.arm_r0,
		context->uc_mcontext.arm_r1,
		context->uc_mcontext.arm_r2,
		context->uc_mcontext.arm_r3);
	Log::SendCrashLog("segv", "r4: 0x%x, r5: 0x%x, r6: 0x%x, r7: 0x%x",
		context->uc_mcontext.arm_r4,
		context->uc_mcontext.arm_r5,
		context->uc_mcontext.arm_r6,
		context->uc_mcontext.arm_r7);
	Log::SendCrashLog("segv", "r8: 0x%x, r9: 0x%x, sl: 0x%x, fp: 0x%x",
		context->uc_mcontext.arm_r8,
		context->uc_mcontext.arm_r9,
		context->uc_mcontext.arm_r10,
		context->uc_mcontext.arm_fp);
	Log::SendCrashLog("segv", "ip: 0x%x, sp: 0x%x, lr: 0x%x, pc: 0x%x",
		context->uc_mcontext.arm_ip,
		context->uc_mcontext.arm_sp,
		context->uc_mcontext.arm_lr,
		context->uc_mcontext.arm_pc);

	Log::SendCrashLog("segv", "backtrace:");
	Log::SendCrashLog("segv", "1: libc.so (PC: 0x%X, LR: 0x%X)", context->uc_mcontext.arm_pc - android_sdk(), context->uc_mcontext.arm_lr - android_sdk());
	Log::SendCrashLog("segv", "2: libGTASA.so (PC: 0x%X, LR: 0x%X)", context->uc_mcontext.arm_pc - g_libGTASA, context->uc_mcontext.arm_lr - g_libGTASA);
	Log::SendCrashLog("segv", "3: libprojectsa.so (PC: 0x%X, LR: 0x%X)", context->uc_mcontext.arm_pc - g_libPSA, context->uc_mcontext.arm_lr - g_libPSA);

	PrintAddressInfo("segv", (void*)(context->uc_mcontext.arm_pc), (void*)(context->uc_mcontext.arm_lr));

	std::terminate();
	return;
}

void Log::handler1(int signum, siginfo_t *info, void* contextPtr)
{
	ucontext* context = (ucontext_t*)contextPtr;

	if (Log::act1_old.sa_sigaction)
		Log::act1_old.sa_sigaction(signum, info, contextPtr);

	Log::SendCrashLog("abrt", "======= Project SA CRASH LOG =======");
	
	Log::SendCrashLog("abrt", "Exception At Address: 0x%X", info->si_addr);
	
	Log::SendCrashLog("abrt", "SCM Op: 0x%X, lDbg: %d", dwScmOpcodeDebug, bScmLocalDebug);

	Log::SendCrashLog("abrt", "libGTASA.so base address: 0x%X", g_libGTASA);
	Log::SendCrashLog("abrt", "libprojectsa.so base address: 0x%X", g_libPSA);

	Log::SendCrashLog("abrt", "Register states:");
	Log::SendCrashLog("abrt", "r0: 0x%X, r1: 0x%X, r2: 0x%X, r3: 0x%X",
		context->uc_mcontext.arm_r0,
		context->uc_mcontext.arm_r1,
		context->uc_mcontext.arm_r2,
		context->uc_mcontext.arm_r3);
	Log::SendCrashLog("abrt", "r4: 0x%x, r5: 0x%x, r6: 0x%x, r7: 0x%x",
		context->uc_mcontext.arm_r4,
		context->uc_mcontext.arm_r5,
		context->uc_mcontext.arm_r6,
		context->uc_mcontext.arm_r7);
	Log::SendCrashLog("abrt", "r8: 0x%x, r9: 0x%x, sl: 0x%x, fp: 0x%x",
		context->uc_mcontext.arm_r8,
		context->uc_mcontext.arm_r9,
		context->uc_mcontext.arm_r10,
		context->uc_mcontext.arm_fp);
	Log::SendCrashLog("abrt", "ip: 0x%x, sp: 0x%x, lr: 0x%x, pc: 0x%x",
		context->uc_mcontext.arm_ip,
		context->uc_mcontext.arm_sp,
		context->uc_mcontext.arm_lr,
		context->uc_mcontext.arm_pc);

	Log::SendCrashLog("abrt", "backtrace:");
	Log::SendCrashLog("abrt", "1: libc.so (PC: 0x%X, LR: 0x%X)", context->uc_mcontext.arm_pc - android_sdk(), context->uc_mcontext.arm_lr - android_sdk());
	Log::SendCrashLog("abrt", "2: libGTASA.so (PC: 0x%X, LR: 0x%X)", context->uc_mcontext.arm_pc - g_libGTASA, context->uc_mcontext.arm_lr - g_libGTASA);
	Log::SendCrashLog("abrt", "3: libprojectsa.so (PC: 0x%X, LR: 0x%X)", context->uc_mcontext.arm_pc - g_libPSA, context->uc_mcontext.arm_lr - g_libPSA);
	
	PrintAddressInfo("abrt", (void*)(context->uc_mcontext.arm_pc), (void*)(context->uc_mcontext.arm_lr));

	std::terminate();
	return;
}

void Log::handler2(int signum, siginfo_t *info, void* contextPtr)
{
	ucontext* context = (ucontext_t*)contextPtr;

	if (Log::act2_old.sa_sigaction)
		Log::act2_old.sa_sigaction(signum, info, contextPtr);

	Log::SendCrashLog("fpe", "======= Project SA CRASH LOG =======");
	
	Log::SendCrashLog("fpe", "Exception At Address: 0x%X", info->si_addr);

	Log::SendCrashLog("fpe", "SCM Op: 0x%X, lDbg: %d", dwScmOpcodeDebug, bScmLocalDebug);

	Log::SendCrashLog("fpe", "libGTASA.so base address: 0x%X", g_libGTASA);
	Log::SendCrashLog("fpe", "libprojectsa.so base address: 0x%X", g_libPSA);

	Log::SendCrashLog("fpe", "Register states:");
	Log::SendCrashLog("fpe", "r0: 0x%X, r1: 0x%X, r2: 0x%X, r3: 0x%X",
		context->uc_mcontext.arm_r0,
		context->uc_mcontext.arm_r1,
		context->uc_mcontext.arm_r2,
		context->uc_mcontext.arm_r3);
	Log::SendCrashLog("fpe", "r4: 0x%x, r5: 0x%x, r6: 0x%x, r7: 0x%x",
		context->uc_mcontext.arm_r4,
		context->uc_mcontext.arm_r5,
		context->uc_mcontext.arm_r6,
		context->uc_mcontext.arm_r7);
	Log::SendCrashLog("fpe", "r8: 0x%x, r9: 0x%x, sl: 0x%x, fp: 0x%x",
		context->uc_mcontext.arm_r8,
		context->uc_mcontext.arm_r9,
		context->uc_mcontext.arm_r10,
		context->uc_mcontext.arm_fp);
	Log::SendCrashLog("fpe", "ip: 0x%x, sp: 0x%x, lr: 0x%x, pc: 0x%x",
		context->uc_mcontext.arm_ip,
		context->uc_mcontext.arm_sp,
		context->uc_mcontext.arm_lr,
		context->uc_mcontext.arm_pc);

	Log::SendCrashLog("fpe", "backtrace:");
	Log::SendCrashLog("fpe", "1: libc.so (PC: 0x%X, LR: 0x%X)", context->uc_mcontext.arm_pc - android_sdk(), context->uc_mcontext.arm_lr - android_sdk());
	Log::SendCrashLog("fpe", "2: libGTASA.so (PC: 0x%X, LR: 0x%X)", context->uc_mcontext.arm_pc - g_libGTASA, context->uc_mcontext.arm_lr - g_libGTASA);
	Log::SendCrashLog("fpe", "3: libprojectsa.so (PC: 0x%X, LR: 0x%X)", context->uc_mcontext.arm_pc - g_libPSA, context->uc_mcontext.arm_lr - g_libPSA);

	PrintAddressInfo("fpe", (void*)(context->uc_mcontext.arm_pc), (void*)(context->uc_mcontext.arm_lr));

	std::terminate();
	return;
}

void Log::handler3(int signum, siginfo_t *info, void* contextPtr)
{
	ucontext* context = (ucontext_t*)contextPtr;

	if (Log::act3_old.sa_sigaction)
		Log::act3_old.sa_sigaction(signum, info, contextPtr);

	Log::SendCrashLog("bus", "======= Project SA CRASH LOG =======");
	
	Log::SendCrashLog("bus", "Exception At Address: 0x%X", info->si_addr);

	Log::SendCrashLog("bus", "SCM Op: 0x%X, lDbg: %d", dwScmOpcodeDebug, bScmLocalDebug);

	Log::SendCrashLog("bus", "libGTASA.so base address: 0x%X", g_libGTASA);
	Log::SendCrashLog("bus", "libprojectsa.so base address: 0x%X", g_libPSA);

	Log::SendCrashLog("bus", "Register states:");
	Log::SendCrashLog("bus", "r0: 0x%X, r1: 0x%X, r2: 0x%X, r3: 0x%X",
		context->uc_mcontext.arm_r0,
		context->uc_mcontext.arm_r1,
		context->uc_mcontext.arm_r2,
		context->uc_mcontext.arm_r3);
	Log::SendCrashLog("bus", "r4: 0x%x, r5: 0x%x, r6: 0x%x, r7: 0x%x",
		context->uc_mcontext.arm_r4,
		context->uc_mcontext.arm_r5,
		context->uc_mcontext.arm_r6,
		context->uc_mcontext.arm_r7);
	Log::SendCrashLog("bus", "r8: 0x%x, r9: 0x%x, sl: 0x%x, fp: 0x%x",
		context->uc_mcontext.arm_r8,
		context->uc_mcontext.arm_r9,
		context->uc_mcontext.arm_r10,
		context->uc_mcontext.arm_fp);
	Log::SendCrashLog("bus", "ip: 0x%x, sp: 0x%x, lr: 0x%x, pc: 0x%x",
		context->uc_mcontext.arm_ip,
		context->uc_mcontext.arm_sp,
		context->uc_mcontext.arm_lr,
		context->uc_mcontext.arm_pc);

	Log::SendCrashLog("bus", "backtrace:");
	Log::SendCrashLog("bus", "1: libc.so (PC: 0x%X, LR: 0x%X)", context->uc_mcontext.arm_pc - android_sdk(), context->uc_mcontext.arm_lr - android_sdk());
	Log::SendCrashLog("bus", "2: libGTASA.so (PC: 0x%X, LR: 0x%X)", context->uc_mcontext.arm_pc - g_libGTASA, context->uc_mcontext.arm_lr - g_libGTASA);
	Log::SendCrashLog("bus", "3: libprojectsa.so (PC: 0x%X, LR: 0x%X)", context->uc_mcontext.arm_pc - g_libPSA, context->uc_mcontext.arm_lr - g_libPSA);

	PrintAddressInfo("bus", (void*)(context->uc_mcontext.arm_pc), (void*)(context->uc_mcontext.arm_lr));

	std::terminate();
	return;
}

uint32_t Log::_session_start_tick = 0;
struct sigaction Log::act_old;
struct sigaction Log::act1_old;
struct sigaction Log::act2_old;
struct sigaction Log::act3_old;