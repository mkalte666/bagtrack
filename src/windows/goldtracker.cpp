// licence note at the end of the file

#include <imgui.h>

#include "../imgui_fmt.h"
#include "../tpinfo.h"
#include "goldtracker.h"
#include "timeselector.h"

GoldTracker::GoldTracker()
    : Window("Gold Tracker")
    , lastFullUpdate(std::chrono::steady_clock::now())
{
}

void GoldTracker::update(Settings& settings, ItemTracker& tracker, InfoCache& cache) noexcept
{
    using namespace std::chrono_literals;
    checkShownStateAgainstSettings(settings);
    if (!shown) {
        return;
    }

    ImGui::SetNextWindowSize(ImVec2(DefaultSquareWindowSize, DefaultSquareWindowSize), ImGuiCond_FirstUseEver);
    ImGui::Begin(name.c_str(), &shown);

    auto oldRefId = referenceId;
    referenceId = timeSelector(tracker, referenceId);

    if (std::chrono::steady_clock::now() - lastFullUpdate > 10s || currentValue == 0 || oldRefId != referenceId) {
        fullValueUpdate(tracker, cache);
        lastFullUpdate = std::chrono::steady_clock::now();
    }

    const TrackerState currentState = tracker.getCurrentState();
    const TrackerState referenceState = tracker.getState(referenceId);
    const TrackerState deltaState = tracker.getDeltaState(referenceId, tracker.getCurrentStateId());

    // table header
    ImGui::Columns(4);
    ImGui::TextFmt("What?");
    ImGui::NextColumn();
    ImGui::TextFmt("At Start");
    ImGui::NextColumn();
    ImGui::TextFmt("Currently");
    ImGui::NextColumn();
    ImGui::TextFmt("Gain");
    ImGui::NextColumn();
    ImGui::Separator();

    // account coins
    ImGui::TextFmt("Coins");
    ImGui::NextColumn();
    ImGui::TextFmt(prettyGoldValue(referenceState.coins));
    ImGui::NextColumn();
    ImGui::TextFmt(prettyGoldValue(currentState.coins));
    ImGui::NextColumn();
    ImGui::TextFmt(prettyGoldValue(deltaState.coins));
    ImGui::NextColumn();

    // collected value of items
    ImGui::TextFmt("Item Value");
    ImGui::NextColumn();
    ImGui::TextFmt(prettyGoldValue(referenceValue));
    ImGui::NextColumn();
    ImGui::TextFmt(prettyGoldValue(currentValue));
    ImGui::NextColumn();
    ImGui::TextFmt(prettyGoldValue(currentValue - referenceValue));
    ImGui::NextColumn();
    ImGui::Separator();

    // total values
    ImGui::TextFmt("Total");
    ImGui::NextColumn();
    ImGui::TextFmt(prettyGoldValue(referenceValue + referenceState.coins));
    ImGui::NextColumn();
    ImGui::TextFmt(prettyGoldValue(currentValue + currentState.coins));
    ImGui::NextColumn();
    ImGui::TextFmt(prettyGoldValue((currentValue - referenceValue) + deltaState.coins));
    ImGui::NextColumn();

    // done
    ImGui::End();
}

void GoldTracker::drawMainMenu() noexcept
{
    if (ImGui::BeginMenu("Trackers")) {
        if (ImGui::MenuItem("Gold Tracker")) {
            shown = true;
        }
        ImGui::EndMenu();
    }
}

void GoldTracker::fullValueUpdate(ItemTracker& tracker, InfoCache& cache) noexcept
{
    const auto referenceState = tracker.getState(referenceId);
    const auto currentState = tracker.getCurrentState();

    auto calcFun = [&cache](const ItemIdMap& items) -> int64_t {
        int64_t value = 0;
        for (const auto& item : items) {
            value += cache.estimateItemValue(item.first) * static_cast<int64_t>(item.second);
        }
        return value;
    };

    referenceValue = calcFun(referenceState.items);
    currentValue = calcFun(currentState.items);
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