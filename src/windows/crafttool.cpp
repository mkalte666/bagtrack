// licence note at the end of the file

#include "crafttool.h"
#include <imgui.h>
#include <imgui_fmt.h>
#include <misc/cpp/imgui_stdlib.h>

CraftTool::CraftTool() noexcept
    : Window("Craft Tool")
{
}

void CraftTool::update(Settings&, ItemTracker&, InfoCache& cache) noexcept
{
    if (!shown) {
        return;
    }

    ImGui::Begin(name.c_str(), &shown);
    if (ImGui::Button("Add New...")) {
        ImGui::OpenPopup("AddItemPopup");
    }
    ImGui::SetNextWindowSizeConstraints(ImVec2(300.0F, 200.0F), ImVec2(500.0F, 500.0F));
    if (ImGui::BeginPopup("AddItemPopup")) {
        const std::string oldSearch = addItemSearchTerm;
        ImGui::InputText("Search##CraftItemSearch", &addItemSearchTerm, ImGuiInputTextFlags_EnterReturnsTrue);
        if (oldSearch != addItemSearchTerm) {
            lookupItems = cache.findItems(addItemSearchTerm);
        }

        if (ImGui::BeginTable("Search Table", 2, ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY)) {
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableSetupColumn("Item");
            ImGui::TableSetupColumn("Recipes");
            ImGui::TableHeadersRow();
            constexpr size_t maxCount = 20;
            size_t count = 0;
            for (const auto id : lookupItems) {
                ImGui::TableNextColumn();
                ImGui::TextFmt("{}", cache.getItemInfo(id).name);
                ImGui::TableNextColumn();
                const auto& recipes = cache.findRecipesForItem(id);
                if (recipes.empty()) {
                    ImGui::TextFmt("Cannot be Crafted");
                } else {
                    for (const auto recipeId : recipes) {
                        ImGui::PushID(static_cast<int>(recipeId));
                        const auto& recipe = cache.getRecipe(recipeId);
                        ImGui::TextFmt("{}", recipe.type);
                        ImGui::SameLine();
                        ImGui::Button("Add");
                        ImGui::PopID();
                    }
                }
                ++count;
                if (count >= maxCount) {
                    break;
                }
            }
            ImGui::EndTable();
        }
        ImGui::EndPopup();
    }
    ImGui::End();
}

void CraftTool::drawMainMenu() noexcept
{
    if (ImGui::BeginMenu("Items")) {
        if (ImGui::MenuItem("Crafting")) {
            shown = true;
        }
        ImGui::EndMenu();
    }
}

/*
 * This file is part bagtrack
 * Copyright (c) 2020 Malte Kießling
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
