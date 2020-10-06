// licence note at the end of the file

#include "itemselector.h"

#include "../fixfmt.h"
#include "timeselector.h"
#include <imgui.h>

ItemSelector::ItemSelector()
    : Window("Track with Filter")
{
}

void ItemSelector::update(Settings& settings, ItemTracker& tracker, InfoCache& cache) noexcept
{
    if (!shown) {
        return;
    }

    editFilter(settings, tracker, cache);

    const auto state = tracker.getCurrentState();
    ImGui::Begin(name.c_str(), &shown);
    if (ImGui::Button("Edit Filter")) {
        selectorWidgetShown = !selectorWidgetShown;
    }
    referenceId = timeSelector(tracker, referenceId);

    const auto deltaState = tracker.getDeltaState(referenceId, tracker.getCurrentStateId());
    listItems(widgetState, deltaState.items, cache, 0, settings.getTrackedItems());
    ImGui::End();
}

void ItemSelector::drawMainMenu() noexcept
{
    if (ImGui::BeginMenu("Trackers")) {
        if (ImGui::MenuItem("Track with Filter")) {
            shown = true;
        }
        ImGui::EndMenu();
    }
}

void ItemSelector::editFilter(Settings& settings, ItemTracker& tracker, InfoCache& cache) noexcept
{
    if (!selectorWidgetShown) {
        return;
    }
    ImGui::Begin("Edit Filter", &selectorWidgetShown);
    ImGui::Columns(2);
    ImGui::BeginChild("SelectorList");
    const auto state = tracker.getCurrentState();
    auto trackedItems = settings.getTrackedItems();
    if (auto id = listItems(selectorWidgetState, state.items, cache, 3, std::set<ItemId>(), false); id != 0) {
        trackedItems.insert(id);
        settings.setTrackedItems(trackedItems);
    }
    ImGui::EndChild();
    ImGui::NextColumn();
    ImGui::Text("Currently Tracked Items"); // NOLINT
    ImGui::Separator();
    ImGui::BeginChild("TrackList");
    const auto trackedItemsCopy = trackedItems;
    for (const auto& id : trackedItemsCopy) {
        const auto& info = cache.getItemInfo(id);
        if (ImGui::Selectable(info.name.c_str())) {
            trackedItems.erase(id);
            settings.setTrackedItems(trackedItems);
        }
    }
    ImGui::EndChild();
    ImGui::Columns(1);
    ImGui::End();
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
