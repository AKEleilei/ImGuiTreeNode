/**
  ******************************************************************************
  * @file           : ImGuiTreeNode.h
  * @author         : Benjamin Enger
  * @brief          : None
  * @description    : imgui 自定义的带图标的 TreeNode, 实现代码参考来自: https://github.com/ocornut/imgui/issues/282
  *                   对其进行了修改
  * @date           : 2024/3/5 16:58:59
  ******************************************************************************
  */

#pragma once

#include <imgui_internal.h>

namespace spider {

inline bool TreeNode(const char* label, ImTextureID user_texture_id, bool arrow = true)
{
    ImGuiContext& imGuiContext = *ImGui::GetCurrentContext();
    ImGuiWindow* window = imGuiContext.CurrentWindow;

    ImGuiID id = window->GetID(label);
    ImVec2 pos = window->DC.CursorPos;
    ImRect bb(pos, ImVec2(pos.x + ImGui::GetContentRegionAvail().x, pos.y + imGuiContext.FontSize + imGuiContext.Style.FramePadding.y * 2));
    bool opened = ImGui::TreeNodeBehaviorIsOpen(id, ImGuiTreeNodeFlags_None);
    bool hovered, held;

    // 点击
    if (ImGui::ButtonBehavior(bb, id, &hovered, &held, ImGuiButtonFlags_PressedOnClick))
        window->DC.StateStorage->SetInt(id, opened ? 0 : 1);

    // 绘制鼠标悬浮和点击的颜色
    if (hovered || held)
        window->DrawList->AddRectFilled(bb.Min, bb.Max, ImGui::GetColorU32(held ? ImGuiCol_HeaderActive : ImGuiCol_HeaderHovered));

    // 绘制箭头图标, 并且让其位置和文本对齐
    if (arrow)
    {
        if (opened)
        {
            ImGui::RenderArrow(window->DrawList, ImVec2(pos.x,  pos.y + imGuiContext.Style.FramePadding.y), ImGui::GetColorU32(ImGuiCol_Text), ImGuiDir_Down);
        }
        else
        {
            ImGui::RenderArrow(window->DrawList, ImVec2(pos.x,  pos.y + imGuiContext.Style.FramePadding.y), ImGui::GetColorU32(ImGuiCol_Text), ImGuiDir_Right);
        }
    }

    // Icon, text
    float button_sz = imGuiContext.FontSize + imGuiContext.Style.FramePadding.y * 2;
    pos.x = pos.x + button_sz;
    window->DrawList->AddImage(user_texture_id, pos, ImVec2(pos.x + button_sz, pos.y + button_sz));
    ImGui::RenderText(ImVec2(pos.x + button_sz + imGuiContext.Style.ItemInnerSpacing.x, pos.y + imGuiContext.Style.FramePadding.y), label);

    ImGui::ItemSize(bb, imGuiContext.Style.FramePadding.y * 2);
    ImGui::ItemAdd(bb, id);

    if (opened)
        ImGui::TreePush(label);

    return opened;
}

inline void TreePop()
{
    ImGui::TreePop();
}

}   // spider