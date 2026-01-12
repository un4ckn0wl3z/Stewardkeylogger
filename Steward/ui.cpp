#include "ui.h"
#include "../common/common.h"
#include "pipe_server.h"
#include "hook_control.h"
#include "theme.h"
#include "imgui.h"
#include <string>
#include <fstream>

extern bool     g_Paused;
extern bool     g_AutoScroll;
extern char     g_SearchBuf[256];
extern HMODULE  g_HookDll;

bool g_Paused = false;
bool g_AutoScroll = true;
char g_SearchBuf[256] = "";
bool showAboutModal = false;


void RenderUI() {
    ProcessNewEntries();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("Steward KL - Research Purpose Keylogger", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar);

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Install Hook")) InstallGlobalHook();
            if (ImGui::MenuItem("Remove Hook"))  RemoveGlobalHook();
            if (ImGui::MenuItem("Save Logs")) {
                std::wofstream f("kl-down.txt", std::ios::app);
                std::lock_guard<std::mutex> lk(g_LogMutex);
                for (auto& e : g_LogEntries) f << e.rawLine << L"\n";
            }
            ImGui::EndMenu();

        }
        if (ImGui::BeginMenu("About")) {
            if (ImGui::MenuItem("About StewardKL")) {
                showAboutModal = true;
            }
            ImGui::EndMenu();
        }
        if (showAboutModal) {
            ImGui::OpenPopup("About##modal");
            showAboutModal = false;
        }
        if (ImGui::BeginPopupModal("About##modal", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.4f, 1.0f),
                "[StewardKL] - Research Purpose Keylogger");
            ImGui::Separator();

            ImGui::TextWrapped(
                "An experimental Windows input monitoring tool that uses global keyboard hooks "
                "and DLL injection to study low-level keyboard event handling in controlled, "
                "consent-based environments."
            );

            ImGui::Separator();
            ImGui::Text("Powered by:");
            ImGui::BulletText("Dear ImGui");
            ImGui::BulletText("DirectX 11");

            ImGui::Separator();
            ImGui::BulletText("Developer: un4ckn0wl3z");

            ImGui::Separator();
            ImGui::TextDisabled("For educational and research use only. User consent required.");

            if (ImGui::Button("Close"))
                ImGui::CloseCurrentPopup();

            ImGui::EndPopup();
        }
        ImGui::Text(" | Entries: %zu", g_LogEntries.size());
        if (g_HookDll) ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), " ACTIVE");
        ImGui::EndMenuBar();
    }



    ImGui::Checkbox("Pause", &g_Paused); ImGui::SameLine();
    ImGui::Checkbox("Auto-scroll", &g_AutoScroll); ImGui::SameLine();
    if (ImGui::Button("Clear")) {
        std::lock_guard<std::mutex> lk(g_LogMutex);
        g_LogEntries.clear();
    }
    ImGui::SameLine();
    ImGui::InputText("Filter", g_SearchBuf, sizeof(g_SearchBuf));

    ImGui::BeginChild("log", ImVec2(0, 0), true);

    std::string s(g_SearchBuf);
    std::wstring ws;
    int clen = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);
    if (clen > 0) {
        ws.resize(clen - 1);
        MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, &ws[0], clen);
    }

    ImGuiListClipper clipper;
    clipper.Begin((int)g_LogEntries.size());
    while (clipper.Step()) {
        for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i) {
            const auto& e = g_LogEntries[i];
            if (!ws.empty() && e.rawLine.find(ws) == std::wstring::npos) continue;

            std::string txt;
            int len = WideCharToMultiByte(CP_UTF8, 0, e.rawLine.c_str(), -1, nullptr, 0, nullptr, nullptr);
            if (len > 0) {
                txt.resize(len - 1);
                WideCharToMultiByte(CP_UTF8, 0, e.rawLine.c_str(), -1, &txt[0], len, nullptr, nullptr);
            }

            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.95f, 0.1f, 1.0f));
            ImGui::TextUnformatted(txt.c_str());
            ImGui::PopStyleColor();
        }
    }
    clipper.End();

    if (g_AutoScroll)
        ImGui::SetScrollHereY(1.0f);

    ImGui::EndChild();
    ImGui::End();
}