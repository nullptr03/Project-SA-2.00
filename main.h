#pragma once

#include <jni.h>
#include <typeinfo>
#include <android/log.h>
#include <ucontext.h>
#include <pthread.h>
#include <thread>

#include <malloc.h>
#include <cstdlib>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <list>
#include <thread>
#include <chrono>
#include <cstdarg>
#include <iterator>
#include <set>
#include <cstdint>
#include <algorithm>
#include <random>
#include <iomanip>
#include <memory>
#include <functional>
#include <map>
#include <array>
#include <sys/mman.h>
#include <unistd.h>
#include <unordered_map>
#include <dlfcn.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PSA_VERSION	    "0.1.0a"
#define GAME_VERSION	"2.00"

#include "util/util.h"

extern uintptr_t g_libPSA;
extern uintptr_t g_libGTASA;

uint32_t GetTickCount();