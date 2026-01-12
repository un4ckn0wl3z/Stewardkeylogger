#pragma once
#include <windows.h>

extern HMODULE g_HookDll;

bool InstallGlobalHook();
void RemoveGlobalHook();