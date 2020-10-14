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
    ImGui::BeginChild("##tableHeader", ImVec2(0, ImGui::GetTextLineHeightWithSpacing()));
    ImGui::Columns(cols);
    float columWidth = ImGui::GetWindowWidth() / static_cast<float>(cols);
    for (int i = 0; i < cols; ++i) {
        ImGui::SetColumnWidth(i, columWidth);
    }
    ImGui::TextWrappedFmt("Name");
    ImGui::NextColumn();
    ImGui::TextWrappedFmt("Count");
    ImGui::NextColumn();
    if (showInfos) {
        ImGui::TextWrappedFmt("Sell Value");
        ImGui::NextColumn();
        ImGui::TextWrappedFmt("Buy Value");
        ImGui::NextColumn();
    }
    ImGui::Columns(1);
    ImGui::EndChild();

    ImGui::BeginChild("##itemwidget");
    ImGui::Columns(cols);
    for (int i = 0; i < cols; ++i) {
        ImGui::SetColumnWidth(i, columWidth);
    }
    // only display when we have something in the filter
    if (state.filter.size() >= minFilterLetters) {

        for (const auto& id : state.filtered) {
            const ItemInfo& info = cache.getItemInfo(id);
            if (!idFilter.empty() && idFilter.count(id) == 0 && !info.checkIfPrecursor()) {
                continue;
            }
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
            ImGui::NextColumn();
            int64_t count = 0;
            if (const auto iter = items.find(id); iter != items.end()) {
                count = iter->second;
            }
            ImGui::TextWrappedFmt("{}", count);
            ImGui::NextColumn();
            if (showInfos) {
                if (info.checkIfBound()) {
                    ImGui::TextWrappedFmt("Bound Item");
                } else {
                    ImGui::TextWrappedFmt(prettyGoldValue(tpInfo.sellValue));
                }
                ImGui::NextColumn();
                if (info.checkIfBound()) {
                    ImGui::TextWrappedFmt("Bound Item");
                } else {
                    ImGui::TextWrappedFmt(prettyGoldValue(tpInfo.buyValue));
                }
                ImGui::NextColumn();
            }
        }
    } else {
        ImGui::TextWrappedFmt("Type something to search for items!");
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
