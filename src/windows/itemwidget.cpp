// licence note at the end of the file

#include "itemwidget.h"
#include "../filteritems.h"
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

ItemId listItems(ItemWidgetState& state, const ItemIdMap& items, InfoCache& cache)
{
    ItemId resultId = 0;
    ImGui::SetNextItemWidth(200);
    ImGui::InputText("Filter", &state.filter);
    ImGui::SameLine();
    const auto sortStrats = getSortStrategies();
    auto currentStrat = static_cast<size_t>(state.sortStrategy);
    if (ImGui::BeginCombo("Sort", sortStrats[currentStrat].c_str())) {
        for (size_t i = 0; i < sortStrats.size(); ++i) {
            if (ImGui::Selectable(sortStrats[i].c_str(), currentStrat == i)) {
                state.sortStrategy = static_cast<SortStrategy>(i);
            }
        }
        ImGui::EndCombo();
    }
    const auto sorted = sortItems(items, cache, state.sortStrategy);
    const auto filtered = filterItems(sorted, cache, state.filter);

    ImGui::BeginChild("##itemwidget");
    ImGui::Columns(2);
    for (const auto& id : filtered) {
        const ItemInfo& info = cache.getItemInfo(id);
        if (ImGui::Selectable(fmt::format("{}", info.name).c_str(), false)) {
            resultId = id;
        }
        ImGui::NextColumn();
        ImGui::Text("%s", fmt::format("{}", items.at(id)).c_str());
        ImGui::NextColumn();
    }
    ImGui::Columns(1);
    ImGui::EndChild();

    return resultId;
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
