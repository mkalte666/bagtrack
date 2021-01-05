// licence note at the end of the file

#include "itemwidget.h"
#include "../filteritems.h"
#include "../imgui_fmt.h"
#include "../sdlhelpers.h"
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
    if (ImGui::BeginPopupContextItem()) {
        if (ImGui::Selectable("Paste")) {
            newFilter = getClipboard();
        }
        ImGui::EndPopup();
    }
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

    // draw table header
    if (ImGui::BeginTable("##ItemTable", cols, ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable)) {
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Count");

        if (showInfos) {
            ImGui::TableSetupColumn("Sell Value");
            ImGui::TableSetupColumn("Buy Value");
        }
        ImGui::TableHeadersRow();

        // only display when we have something in the filter
        if (state.filter.size() >= minFilterLetters) {
            for (const auto& id : state.filtered) {
                const ItemInfo& info = cache.getItemInfo(id);
                if (!idFilter.empty() && idFilter.count(id) == 0 && !info.checkIfPrecursor()) {
                    continue;
                }
                // after filtering, or we brek our table!
                ImGui::TableNextColumn();

                if (info.checkIfPrecursor()) {
                    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0x8FU, 0xFFU, 0xF8U, 0xFFU));
                }
                const TpInfo& tpInfo = cache.getTpInfo(id);
                if (ImGui::Selectable(fmt::format("{}", info.name).c_str(), false)) {
                    resultId = id;
                }
                if (info.checkIfPrecursor()) {
                    ImGui::PopStyleColor();
                }
                ImGui::TableNextColumn();
                int64_t count = 0;
                if (const auto iter = items.find(id); iter != items.end()) {
                    count = iter->second;
                }
                ImGui::TextWrappedFmt("{}", count);
                if (showInfos) {
                    ImGui::TableNextColumn();
                    if (info.checkIfBound()) {
                        ImGui::TextWrappedFmt("Bound Item");
                    } else {
                        ImGui::TextWrappedFmt(prettyGoldValue(tpInfo.sellValue));
                    }
                    ImGui::TableNextColumn();
                    if (info.checkIfBound()) {
                        ImGui::TextWrappedFmt("Bound Item");
                    } else {
                        ImGui::TextWrappedFmt(prettyGoldValue(tpInfo.buyValue));
                    }
                }
            }
        } else {
            ImGui::TableNextColumn();
            ImGui::TextWrappedFmt("Type something to search for items!");
        }

        ImGui::EndTable();
    }
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
