#include "pipe_server.h"
#include "../common/common.h"
#include <sstream>
#include <thread>

std::vector<KeyLogEntry> g_LogEntries;
std::queue<KeyLogEntry>  g_NewEntries;
std::mutex               g_LogMutex;

DWORD WINAPI PipeServerThread(LPVOID) {
    while (true) {
        HANDLE pipe = CreateNamedPipeW(PIPE_NAME, PIPE_ACCESS_INBOUND,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            PIPE_UNLIMITED_INSTANCES, 4096, 4096, 0, nullptr);

        if (pipe == INVALID_HANDLE_VALUE) {
            Sleep(1000);
            continue;
        }

        if (!ConnectNamedPipe(pipe, nullptr) && GetLastError() != ERROR_PIPE_CONNECTED) {
            CloseHandle(pipe);
            Sleep(1000);
            continue;
        }

        std::thread([pipe] {
            wchar_t buf[BUFFER_SIZE];
            DWORD bytes;

            while (ReadFile(pipe, buf, sizeof(buf) - sizeof(wchar_t), &bytes, nullptr) && bytes > 0) {
                buf[bytes / sizeof(wchar_t)] = L'\0';

                KeyLogEntry e;
                e.rawLine = buf;
                if (!e.rawLine.empty() && e.rawLine.back() == L'\n')
                    e.rawLine.pop_back();

                std::wistringstream iss(e.rawLine);
                std::wstring token;

                if (std::getline(iss, e.timestamp, L'|')) {
                    e.timestamp.erase(0, e.timestamp.find_first_not_of(L" \t"));
                    e.timestamp.erase(e.timestamp.find_last_not_of(L" \t") + 1);
                }
                if (std::getline(iss, token, L'|')) {
                    size_t p = token.find(L"=");
                    if (p != std::wstring::npos) e.processId = std::wcstoul(token.c_str() + p + 1, nullptr, 10);
                }
                if (std::getline(iss, token, L'|')) {
                    size_t a = token.find(L"\""), b = token.rfind(L"\"");
                    if (a != std::wstring::npos && b != std::wstring::npos && b > a)
                        e.processName = token.substr(a + 1, b - a - 1);
                }
                if (std::getline(iss, token, L'|')) {
                    size_t a = token.find(L"\""), b = token.rfind(L"\"");
                    if (a != std::wstring::npos && b != std::wstring::npos && b > a)
                        e.windowTitle = token.substr(a + 1, b - a - 1);
                }
                if (std::getline(iss, token, L'|')) {
                    size_t eq = token.find(L"="), op = token.find(L"("), cp = token.find(L")");
                    if (eq != std::wstring::npos && op != std::wstring::npos && cp != std::wstring::npos) {
                        e.keyName = token.substr(eq + 1, op - eq - 1);
                        e.keyName.erase(0, e.keyName.find_first_not_of(L" \t"));
                        std::wstring vkstr = token.substr(op + 1, cp - op - 1);
                        e.vkCode = std::wcstoul(vkstr.c_str(), nullptr, 10);
                    }
                }
                if (std::getline(iss, token, L'|')) {
                    size_t p = token.find(L"=");
                    if (p != std::wstring::npos) e.action = token.substr(p + 1);
                }

                {
                    std::lock_guard<std::mutex> lk(g_LogMutex);
                    g_NewEntries.push(std::move(e));
                    while (g_LogEntries.size() > MAX_LOG_ENTRIES)
                        g_LogEntries.erase(g_LogEntries.begin());
                }
            }
            DisconnectNamedPipe(pipe);
            CloseHandle(pipe);
            }).detach();
    }
    return 0;
}

void ProcessNewEntries() {
    std::lock_guard<std::mutex> lk(g_LogMutex);
    while (!g_NewEntries.empty()) {
        g_LogEntries.push_back(std::move(g_NewEntries.front()));
        g_NewEntries.pop();
    }
}