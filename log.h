#pragma once

class Log
{
public:
	static void Initialize();
	static void SendCrashLog(const char *crashId, const char *fmt, ...);

private:
	static void InstallExceptionHandler();
	static void handler(int signum, siginfo_t *info, void* contextPtr);
	static void handler1(int signum, siginfo_t *info, void* contextPtr);
	static void handler2(int signum, siginfo_t *info, void* contextPtr);
	static void handler3(int signum, siginfo_t *info, void* contextPtr);

private:
	static uint32_t _session_start_tick;
	static struct sigaction act_old;
	static struct sigaction act1_old;
	static struct sigaction act2_old;
	static struct sigaction act3_old;
};