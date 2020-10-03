// licence note at the end of the file

#include "trackereverything.h"
#include "../fixfmt.h"
#include "imgui.h"

TrackerEverything::TrackerEverything() noexcept
    : Window("Track Everything")
{
}

void TrackerEverything::update(Settings&, ItemTracker& tracker, InfoCache& cache) noexcept
{
    if (!shown) {
        return;
    }

    ImGui::Begin(name.c_str(), &shown);
    if (ImGui::Button("Reset Tracking")) {
        tracker.resetReferenceState();
    }
    const auto items = tracker.getFilteredDelta();
    listItems(widgetState, items, cache);
    ImGui::End();
}

void TrackerEverything::drawMainMenu() noexcept
{
    if (ImGui::BeginMenu("Trackers")) {
        if (ImGui::MenuItem("Track Everything")) {
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
