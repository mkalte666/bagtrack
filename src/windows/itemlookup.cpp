// licence note at the end of the file

#include "itemlookup.h"
#include "imgui_fmt.h"
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

ItemLookup::ItemLookup() noexcept
    : Window("Item Lookup")
{
}

void ItemLookup::update(Settings&, ItemTracker& tracker, InfoCache& cache) noexcept
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

    if (ImGui::BeginTable("Test Table", 2, ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY)) {
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
            ImGui::TableNextColumn();
            const auto item = cache.getItemInfo(itemId);
            ImGui::TextWrappedFmt("{}", item.name);
            ImGui::TableNextColumn();
            ImGui::TextWrappedFmt("{}", tracker.getCurrentState().items[itemId]);
        }
        ImGui::EndTable();
    }
    ImGui::End();
}

void ItemLookup::drawMainMenu() noexcept
{
    if (ImGui::BeginMenu("Items")) {
        if (ImGui::MenuItem("Look Up")) {
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