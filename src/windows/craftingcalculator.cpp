// licence note at the end of the file

#include "craftingcalculator.h"
#include "imgui_fmt.h"
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

CraftingCalculator::CraftingCalculator() noexcept
    : Window("Crafting Calculator")
{
}

void CraftingCalculator::update(Settings&, ItemTracker& tracker, InfoCache& cache) noexcept
{
    if (!shown) {
        return;
    }

    ImGui::Begin(name.c_str(), &shown);
    std::string searchCopy = searchTerm;
    ImGui::InputText("Search", &searchCopy);
    if (searchCopy != searchTerm) {
        searchTerm = searchCopy;
        searchItems = cache.findItems(searchTerm);
    }

    if (ImGui::BeginTable("Test Table", 2)) {
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Count");
        ImGui::TableHeadersRow();
        constexpr int maxCount = 150;
        int count = 0;
        for (const auto& itemId : searchItems) {
            ++count;
            if (count > maxCount) {
                break;
            }
            ImGui::NextColumn();
            const auto item = cache.getItemInfo(itemId);
            ImGui::TextFmt("{}", item.name);
            ImGui::NextColumn();
            ImGui::TextFmt("{}", tracker.getCurrentState().items[itemId]);
        }
        ImGui::EndTable();
    }
    ImGui::End();
}

void CraftingCalculator::drawMainMenu() noexcept
{
    if (ImGui::MenuItem("Crafting")) {
        shown = true;
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