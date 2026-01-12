#include "theme.h"

void SetupHackerTheme() {
    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowRounding = 0.0f;
    style.ChildRounding = 0.0f;
    style.FrameRounding = 0.0f;
    style.PopupRounding = 0.0f;
    style.ScrollbarRounding = 0.0f;
    style.GrabRounding = 0.0f;
    style.TabRounding = 0.0f;
    style.WindowBorderSize = 1.0f;
    style.FrameBorderSize = 1.0f;

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.97f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.00f, 0.65f, 0.00f, 0.60f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.02f, 0.10f, 0.02f, 0.60f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.04f, 0.35f, 0.04f, 0.80f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.08f, 0.55f, 0.08f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.18f, 0.00f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.40f, 0.00f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.22f, 0.00f, 0.92f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.08f, 0.02f, 0.50f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 0.70f, 0.00f, 0.80f);
    colors[ImGuiCol_Text] = ImVec4(0.10f, 0.95f, 0.10f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.55f, 0.00f, 0.40f);
    colors[ImGuiCol_Button] = ImVec4(0.00f, 0.40f, 0.00f, 0.65f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.00f, 0.75f, 0.00f, 0.85f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 1.00f, 0.10f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.00f, 0.45f, 0.00f, 0.45f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.70f, 0.00f, 0.70f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 0.90f, 0.00f, 0.90f);
    colors[ImGuiCol_Separator] = ImVec4(0.00f, 0.65f, 0.00f, 0.50f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 1.00f, 0.00f, 1.00f);
}