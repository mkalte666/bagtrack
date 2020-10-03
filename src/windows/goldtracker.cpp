// licence note at the end of the file

#include <imgui.h>

#include "../fixfmt.h"
#include "../tpinfo.h"
#include "goldtracker.h"

GoldTracker::GoldTracker()
    : Window("Gold Tracker")
    , lastFullUpdate(std::chrono::steady_clock::now())
{
}

void GoldTracker::update(Settings&, ItemTracker& tracker, InfoCache& cache) noexcept
{
    using namespace std::chrono_literals;
    if (std::chrono::steady_clock::now() - lastFullUpdate > 10s || currentValue == 0) {
        fullValueUpdate(tracker, cache);
        lastFullUpdate = std::chrono::steady_clock::now();
    }

    if (!shown) {
        return;
    }

    ImGui::Begin(name.c_str(), &shown);
    // table header
    ImGui::Columns(4);
    ImGui::Text("What?");
    ImGui::NextColumn();
    ImGui::Text("At Start");
    ImGui::NextColumn();
    ImGui::Text("Currently");
    ImGui::NextColumn();
    ImGui::Text("Gain");
    ImGui::NextColumn();
    ImGui::Separator();

    // account coins
    ImGui::Text("Coins");
    ImGui::NextColumn();
    ImGui::Text("%s", prettyGoldValue(tracker.getReferenceCoins()).c_str());
    ImGui::NextColumn();
    ImGui::Text("%s", prettyGoldValue(tracker.getCurrentCoins()).c_str());
    ImGui::NextColumn();
    ImGui::Text("%s", prettyGoldValue(tracker.getCoinDelta()).c_str());
    ImGui::NextColumn();

    // collected value of items
    ImGui::Text("Item Value");
    ImGui::NextColumn();
    ImGui::Text("%s", prettyGoldValue(referenceValue).c_str());
    ImGui::NextColumn();
    ImGui::Text("%s", prettyGoldValue(currentValue).c_str());
    ImGui::NextColumn();
    ImGui::Text("%s", prettyGoldValue(currentValue - referenceValue).c_str());
    ImGui::NextColumn();
    ImGui::Separator();

    // total values
    ImGui::Text("Total");
    ImGui::NextColumn();
    ImGui::Text("%s", prettyGoldValue(referenceValue + tracker.getReferenceCoins()).c_str());
    ImGui::NextColumn();
    ImGui::Text("%s", prettyGoldValue(currentValue + tracker.getCurrentCoins()).c_str());
    ImGui::NextColumn();
    ImGui::Text("%s", prettyGoldValue((currentValue - referenceValue) + tracker.getCoinDelta()).c_str());
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
    const auto& referenceItems = tracker.getReferenceState();
    const auto& currentItems = tracker.getCurrentState();

    auto calcFun = [&cache](const ItemIdMap& items) -> uint64_t {
        uint64_t value = 0;
        for (const auto& item : items) {
            value += cache.estimateItemValue(item.first) * static_cast<uint64_t>(item.second);
        }
        return value;
    };

    referenceValue = calcFun(referenceItems);
    currentValue = calcFun(currentItems);
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