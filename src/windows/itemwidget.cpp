// licence note at the end of the file

#include "itemwidget.h"
#include "../filteritems.h"
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

ItemId listItems(ItemWidgetState& state, const ItemIdMap& items, InfoCache& cache, size_t minFilterLetters, const std::set<ItemId>& idFilter, bool showInfos)
{
    ItemId resultId = 0; // needed for later
    bool needsResort = false; // set to true if we want to sort again (expensive!)
    bool needsRefilter = false; // set to true if we want to filter again (expensive!)
    // filtering
    std::string newFilter = state.filter;
    ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.6F);
    ImGui::InputText("Filter", &newFilter);
    if (newFilter != state.filter) {
        state.filter = newFilter;
        needsRefilter = true;
    }

    // sorting. element is in the same line, code is more annyoing
    ImGui::SameLine();
    const auto sortStrats = getSortStrategies();
    auto currentStrat = static_cast<size_t>(state.sortStrategy);
    if (ImGui::BeginCombo("Sort", sortStrats[currentStrat].c_str())) {
        for (size_t i = 0; i < sortStrats.size(); ++i) {
            if (ImGui::Selectable(sortStrats[i].c_str(), currentStrat == i)) {
                state.sortStrategy = static_cast<SortStrategy>(i);
                needsResort = true;
            }
        }
        ImGui::EndCombo();
    }

    // changes in the number of items?
    if (state.sorted.size() != items.size()) {
        needsResort = true;
    }

    if (needsResort) {
        state.sorted = sortItems(items, cache, state.sortStrategy);
        needsRefilter = true;
    }
    if (needsRefilter) {
        state.filtered = filterItems(state.sorted, cache, state.filter);
    }
    int cols = 4;
    if (!showInfos) {
        cols = 2;
    }
    ImGui::Columns(cols);
    ImGui::SetColumnWidth(-1, ImGui::GetWindowWidth() / static_cast<float>(cols));
    ImGui::Text("Name");
    ImGui::NextColumn();
    ImGui::Text("Count");
    ImGui::NextColumn();
    if (showInfos) {
        ImGui::Text("Sell Value");
        ImGui::NextColumn();
        ImGui::Text("Buy Value");
        ImGui::NextColumn();
    }
    ImGui::Columns(1);
    ImGui::BeginChild("##itemwidget");
    ImGui::Columns(cols);
    ImGui::SetColumnWidth(-1, ImGui::GetWindowWidth() / static_cast<float>(cols));
    // only display when we have something in the filter
    if (state.filter.size() >= minFilterLetters) {
        for (const auto& id : state.filtered) {
            if (!idFilter.empty() && idFilter.count(id) == 0) {
                continue;
            }
            const ItemInfo& info = cache.getItemInfo(id);
            const TpInfo& tpInfo = cache.getTpInfo(id);
            if (ImGui::Selectable(fmt::format("{}", info.name).c_str(), false)) {
                resultId = id;
            }
            ImGui::NextColumn();
            int64_t count = 0;
            if (const auto iter = items.find(id); iter != items.end()) {
                count = iter->second;
            }
            ImGui::Text("%s", fmt::format("{}", count).c_str());
            ImGui::NextColumn();
            if (showInfos) {
                if (info.checkIfBound()) {
                    ImGui::Text("Bound Item");
                } else {
                    ImGui::Text("%s", fmt::format("{}", prettyGoldValue(tpInfo.sellValue)).c_str());
                }
                ImGui::NextColumn();
                if (info.checkIfBound()) {
                    ImGui::Text("Bound Item");
                } else {
                    ImGui::Text("%s", fmt::format("{}", prettyGoldValue(tpInfo.buyValue)).c_str());
                }
                ImGui::NextColumn();
            }
        }
    } else {
        ImGui::Text("Type something to search for items!");
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
