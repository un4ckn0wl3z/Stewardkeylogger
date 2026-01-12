#pragma once
#include <string>
#include <windows.h>

std::wstring GetTimestamp();
std::wstring GetWindowTitle();
std::wstring GetProcessName(DWORD pid);
std::wstring GetKeyName(DWORD vk, DWORD scan, bool extended);