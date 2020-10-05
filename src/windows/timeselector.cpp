// licence note at the end of the file

#include "timeselector.h"
#include <ctime>
#include <imgui.h>

int64_t nearest(const std::set<int64_t>& numbers, int64_t value)
{
    if (const auto iter = numbers.lower_bound(value); iter != numbers.end()) {
        return *iter;
    }

    return 0;
}

int64_t timeSelector(const ItemTracker& tracker, int64_t oldValue) noexcept
{
    const auto ids = tracker.getStateIds();
    const auto currentId = tracker.getCurrentStateId();
    int64_t value = oldValue;
    if (value == 0) {
        value = currentId;
    }

    auto currentTime = static_cast<time_t>(value * 300);
    ImGui::SetNextItemWidth(-1.0F);
    if (ImGui::BeginCombo("Select Reference Time", std::asctime(std::localtime(&currentTime)))) {
        for (const auto& id : ids) {
            auto idTime = static_cast<time_t>(id * 300);
            ImGui::PushID(static_cast<int>(id));
            if (ImGui::Selectable(std::asctime(std::localtime(&idTime)), id == value)) {
                value = id;
            }
            ImGui::PopID();
        }
        ImGui::EndCombo();
    }

    ImGui::PushItemWidth(30.0F);
    if (ImGui::Button("Now")) {
        value = currentId;
    }
    ImGui::SameLine();
    if (ImGui::Button("5m")) {
        value = currentId - 1;
    }
    ImGui::SameLine();
    if (ImGui::Button("30m")) {
        value = currentId - 6;
    }
    ImGui::SameLine();
    if (ImGui::Button("1h")) {
        value = currentId - 12;
    }
    ImGui::SameLine();
    if (ImGui::Button("12h")) {
        value = currentId - 12 * 12;
    }
    ImGui::SameLine();
    if (ImGui::Button("1d")) {
        value = currentId - 12 * 24;
    }
    ImGui::SameLine();
    if (ImGui::Button("1w")) {
        value = currentId - 12 * 24 * 7;
    }
    ImGui::SameLine();
    if (ImGui::Button("1m")) {
        value = currentId - 12 * 24 * 30;
    }
    ImGui::PopItemWidth();

    value = nearest(ids, value);

    return value;
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