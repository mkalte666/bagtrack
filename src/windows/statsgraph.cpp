// licence note at the end of the file
#include "statsgraph.h"

#include "../fixfmt.h"
#include "../imgui_fmt.h"
#include <SDL.h>
#include <imgui.h>
#include <implot.h>
#include <misc/cpp/imgui_stdlib.h>

void drawTimeStats(const char* name, int64_t current, int64_t min, int64_t max)
{
    ImGui::TextWrappedFmt("{}", name);
    ImGui::Columns(2);
    ImGui::TextWrappedFmt("Current");
    ImGui::NextColumn();
    ImGui::TextWrappedFmt("{}", current);
    ImGui::NextColumn();
    ImGui::TextWrappedFmt("Min");
    ImGui::NextColumn();
    ImGui::TextWrappedFmt("{}", min);
    ImGui::NextColumn();
    ImGui::TextWrappedFmt("Max");
    ImGui::NextColumn();
    ImGui::TextWrappedFmt("{}", max);
    ImGui::NextColumn();
    ImGui::Columns(1);
}

void makeTimeStats(const std::map<int64_t, int64_t>& stats, const int64_t offset, int64_t& current, int64_t& min, int64_t& max)
{
    current = 0;
    min = 0;
    max = 0;
    // worst case, stats is empty.
    if (stats.empty()) {
        return;
    }
    // make current. lower bound, because we will use the closet we can get to our offset!
    const auto currentIter = stats.rbegin();
    const auto currentOffset = stats.lower_bound(currentIter->first - offset);
    // not good
    if (currentOffset == stats.end()) {
        return;
    }
    current = currentIter->second - currentOffset->second;

    // we give ourselfs the range using the iters, and then slide it over the data
    auto iterStart = stats.begin();
    auto iterOffset = stats.find(iterStart->first + offset);
    // we dont have the data range
    if (iterOffset != stats.end()) {
        max = std::numeric_limits<int64_t>::min();
        min = std::numeric_limits<int64_t>::max();
        for (; iterStart != stats.end() && iterOffset != stats.end(); iterStart++, iterOffset++) {
            const int64_t delta = iterOffset->second - iterStart->second;
            min = std::min(delta, min);
            max = std::max(delta, max);
        }
    }
}

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
    ImGui::TextWrappedFmt("Name");
    ImGui::NextColumn();
    ImGui::TextWrappedFmt(info.name);
    ImGui::NextColumn();
    ImGui::Separator();
    ImGui::TextWrappedFmt("Description");
    ImGui::NextColumn();
    ImGui::TextWrappedFmt(info.description);
    ImGui::NextColumn();
    ImGui::Separator();
    ImGui::TextWrappedFmt("Chat Link");
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
    // overall item stats
    ImGui::Separator();
    ImGui::TextWrappedFmt("Min");
    ImGui::NextColumn();
    ImGui::TextWrappedFmt("{}", min);
    ImGui::NextColumn();
    ImGui::TextWrappedFmt("Max");
    ImGui::NextColumn();
    ImGui::TextWrappedFmt("{}", max);
    ImGui::NextColumn();
    ImGui::TextWrappedFmt("Agv");
    ImGui::NextColumn();
    ImGui::TextWrappedFmt("{}", avg);
    ImGui::NextColumn();
    ImGui::Columns(1);
    // timespan based stats
    ImGui::Separator();
    drawTimeStats("5 minute stats", last5Minutes, minDelta, maxDelta);
    ImGui::Separator();
    drawTimeStats("15 minute stats", last15Minutes, minDelta15Minutes, maxDelta15Minutes);
    ImGui::Separator();
    drawTimeStats("1 Hour stats", last60Minutes, minDelta60Minutes, maxDelta60Minutes);
    ImGui::EndChild();

    // draw plot
    ImGui::NextColumn();

    ImPlot::SetNextPlotLimitsY(static_cast<double>(minDelta), static_cast<double>(max) * 1.1);
    ImPlot::SetNextPlotLimitsX(static_cast<double>(idStart * 300), static_cast<double>(idEnd * 300));

    if (ImPlot::BeginPlot(title.c_str(), "Time", "Count", ImVec2(-1, -1), ImPlotAxisFlags_None, ImPlotAxisFlags_Time)) {
        ImPlot::GetStyle().Use24HourClock = true;
        ImPlot::GetStyle().UseLocalTime = true;
        ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, .25F);
        ImPlot::PlotLine<ImS64>(info.name.c_str(), reinterpret_cast<const long long int*>(xVals.data()), reinterpret_cast<const long long int*>(yVals.data()), static_cast<int>(yVals.size()));
        ImPlot::PlotShaded<ImS64>(info.name.c_str(), reinterpret_cast<const long long int*>(xVals.data()), reinterpret_cast<const long long int*>(yVals.data()), static_cast<int>(yVals.size()));
        ImPlot::PopStyleVar();
        const std::string deltaLabel = info.name + " Delta";
        ImPlot::PlotLine<ImS64>(deltaLabel.c_str(), reinterpret_cast<const long long int*>(xVals.data()), reinterpret_cast<const long long int*>(y2Vals.data()), static_cast<int>(yVals.size()));
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
    max = std::numeric_limits<int64_t>::min();
    xVals.clear();
    xVals.reserve(statCache.size());
    yVals.clear();
    yVals.reserve(statCache.size());
    y2Vals.clear();
    y2Vals.reserve(statCache.size());
    tickChars.clear(); // this order!
    tickStrings.clear();
    tickChars.reserve(statCache.size());
    tickStrings.reserve(statCache.size());
    int64_t lastCount = 0;
    // make sure we do not calc a delta for the first value
    // the next loop does both graphs
    if (!statCache.empty()) {
        lastCount = statCache.begin()->second;
    }
    for (const auto& i : statCache) {
        const int64_t id = i.first;
        const int64_t count = i.second;
        const int64_t delta = count - lastCount;
        lastCount = count;

        // update running things
        sum += count;
        min = std::min(count, min);
        max = std::max(count, max);

        // update arrays
        xVals.emplace_back(id * 300); // convert back to unix time
        yVals.emplace_back(count);
        y2Vals.emplace_back(delta);
    }
    avg = static_cast<float>(sum) / static_cast<float>(statCache.size());
    makeTimeStats(statCache, 1, last5Minutes, minDelta, maxDelta);
    makeTimeStats(statCache, 3, last15Minutes, minDelta15Minutes, maxDelta15Minutes);
    makeTimeStats(statCache, 12, last60Minutes, minDelta60Minutes, maxDelta60Minutes);
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
