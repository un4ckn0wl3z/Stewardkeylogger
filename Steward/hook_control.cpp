#include "hook_control.h"
#include <windows.h>
#include <stdio.h>

HMODULE g_HookDll = nullptr;

bool InstallGlobalHook() {
    if (g_HookDll) return true;

    g_HookDll = LoadLibraryW(L"StewardKLHook.dll");
    if (!g_HookDll) {
        wchar_t msg[256];
        swprintf_s(msg, L"Cannot load DLL: %lu", GetLastError());
        MessageBoxW(nullptr, msg, L"Error", MB_ICONERROR);
        return false;
    }

    auto setOpts = (void(*)(bool, bool))GetProcAddress(g_HookDll, "SetLogOptions");
    auto install = (void(*)())GetProcAddress(g_HookDll, "InstallHook");

    if (!setOpts || !install) {
        FreeLibrary(g_HookDll);
        g_HookDll = nullptr;
        MessageBoxW(nullptr, L"Cannot find functions in DLL", L"Error", MB_ICONERROR);
        return false;
    }

    setOpts(false, true);   // file=false, pipe=true
    install();
    return true;
}

void RemoveGlobalHook() {
    if (!g_HookDll) return;

    auto remove = (void(*)())GetProcAddress(g_HookDll, "RemoveHook");
    if (remove) remove();

    FreeLibrary(g_HookDll);
    g_HookDll = nullptr;
}