// licence note at the end of the file

#include "itemselector.h"

#include "../fixfmt.h"
#include <imgui.h>
ItemSelector::ItemSelector()
    : Window("Select Items")
{
}

void ItemSelector::update(Settings&, ItemTracker& tracker, InfoCache& cache) noexcept
{
    if (!shown) {
        return;
    }

    ImGui::Begin(name.c_str(), &shown);
    ImGui::Columns(2);
    auto items = tracker.getCurrentState();
    for (const auto& pair : items) {
        const ItemInfo& info = cache.getItemInfo(pair.first);
        ImGui::Text("%s", info.name.c_str());
        ImGui::NextColumn();
        ImGui::Text("%s", fmt::format("{}", pair.second).c_str());
        ImGui::NextColumn();
    }
    ImGui::Columns(1);
    ImGui::End();
}

void ItemSelector::drawMainMenu() noexcept
{
    if (ImGui::BeginMenu("GW2")) {
        if (ImGui::MenuItem("Select Items to track...")) {
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
