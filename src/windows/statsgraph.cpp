// licence note at the end of the file
#include "statsgraph.h"

#include "../fixfmt.h"
#include <SDL.h>
#include <imgui.h>
#include <implot.h>
#include <misc/cpp/imgui_stdlib.h>

void StatsGraph::update(Settings&, ItemTracker& tracker, InfoCache& cache) noexcept
{
    if (!shown) {
        return;
    }

    if (idStart == 0 || idEnd == 0 || itemId == 0) {
        shown = false;
        return;
    }

    if (needRebuild) {
        needRebuild = false;
        statCache = tracker.getItemStats(itemId, idStart, idEnd);
        rebuildStats();
    }

    const auto& info = cache.getItemInfo(itemId);
    auto title = fmt::format("Stats for {}##{}", info.name, name);
    ImGui::SetNextWindowSize(ImVec2(600.0F, 600.0F), ImGuiCond_FirstUseEver);
    ImGui::Begin(title.c_str(), &shown);
    ImGui::Columns(2);
    ImGui::SetColumnWidth(-1, 250.0F);

    // item info and plot stat table
    ImGui::BeginChild((title + "table area").c_str());
    ImGui::Columns(2);
    ImGui::SetColumnWidth(-1, 100.0F);
    ImGui::TextWrapped("Name");
    ImGui::NextColumn();
    ImGui::TextWrapped("%s", info.name.c_str());
    ImGui::NextColumn();
    ImGui::Separator();
    ImGui::TextWrapped("Description");
    ImGui::NextColumn();
    ImGui::TextWrapped("%s", info.description.c_str());
    ImGui::NextColumn();
    ImGui::Separator();
    ImGui::TextWrapped("Chat Link");
    ImGui::NextColumn();
    std::string chatLinkCopy = info.chatLink;
    ImGui::InputText("##infoclink", &chatLinkCopy, ImGuiInputTextFlags_ReadOnly);
    if (ImGui::BeginPopupContextItem()) {
        if (ImGui::Selectable("Copy")) {
            SDL_SetClipboardText(chatLinkCopy.c_str());
        }
        ImGui::EndPopup();
    }
    ImGui::NextColumn();
    ImGui::Separator();
    ImGui::TextWrapped("Min");
    ImGui::NextColumn();
    ImGui::TextWrapped("%s", fmt::format("{}", min).c_str());
    ImGui::NextColumn();
    ImGui::TextWrapped("Max");
    ImGui::NextColumn();
    ImGui::TextWrapped("%s", fmt::format("{}", max).c_str());
    ImGui::NextColumn();
    ImGui::TextWrapped("Agv");
    ImGui::NextColumn();
    ImGui::TextWrapped("%s", fmt::format("{}", avg).c_str());
    ImGui::NextColumn();
    ImGui::Columns(1);
    ImGui::EndChild();

    // draw plot
    ImGui::NextColumn();
    ImPlot::SetNextPlotLimitsY(0.0, static_cast<double>(max) * 1.1);
    ImPlot::SetNextPlotLimitsX(static_cast<double>(idStart * 300), static_cast<double>(idEnd * 300));
    if (ImPlot::BeginPlot(title.c_str(), "Time", "Count", ImVec2(-1, -1), ImPlotFlags_None, ImPlotAxisFlags_Time)) {
        ImPlot::GetStyle().Use24HourClock = true;
        ImPlot::GetStyle().UseLocalTime = true;
        ImPlot::PlotLine<ImS64>(info.name.c_str(), reinterpret_cast<const long long int*>(xVals.data()), reinterpret_cast<const long long int*>(yVals.data()), static_cast<int>(yVals.size()));
        ImPlot::EndPlot();
    }
    ImGui::End();
}

void StatsGraph::setItem(ItemId id) noexcept
{
    if (id != itemId) {
        itemId = id;
        needRebuild = true;
    }
}

void StatsGraph::setTimeslot(int64_t start, int64_t end) noexcept
{
    if (start != idStart || end != idEnd) {
        needRebuild = true;
        idStart = start;
        idEnd = end;
    }
}

void StatsGraph::rebuildStats() noexcept
{
    int64_t sum = 0;
    min = std::numeric_limits<int64_t>::max();
    max = 0;
    xVals.clear();
    xVals.reserve(statCache.size());
    yVals.clear();
    yVals.reserve(statCache.size());
    tickChars.clear(); // this order!
    tickStrings.clear();
    tickChars.reserve(statCache.size());
    tickStrings.reserve(statCache.size());
    for (const auto& i : statCache) {
        sum += i.second;
        min = std::min(i.second, min);
        max = std::max(i.second, max);
        xVals.emplace_back(i.first * 300); // convert back to unix time
        yVals.emplace_back(i.second);
    }

    avg = static_cast<float>(sum) / static_cast<float>(statCache.size());
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
