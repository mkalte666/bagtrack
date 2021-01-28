// licence note at the end of the file

#include "statusbar.h"
#include "fixfmt.h"
#include <imgui.h>

const ImColor darkGreen(0, 100, 0, 255);
const ImColor darkYellow(80, 80, 0, 255);

void drawPrettyStatus(const std::string& name, const size_t newCount, size_t& savedCount)
{
    const std::string prefix = fmt::format("{} Status:", name);
    if (newCount == 0) {
        const std::string statusName = "IDLE";
        const std::string buttonString = fmt::format("{} {}", prefix, statusName);
        savedCount = 0;
        ImGui::PushStyleColor(ImGuiCol_Button, darkGreen.Value);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, darkGreen.Value);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, darkGreen.Value);
        ImGui::Button(buttonString.c_str());
    } else {
        const std::string statusName = "Download";
        savedCount = std::max(newCount, savedCount);
        const size_t itemsLeft = savedCount - newCount;
        const std::string buttonString = fmt::format("{} {}({}/{})", prefix, statusName, itemsLeft, savedCount);
        ImGui::PushStyleColor(ImGuiCol_Button, darkYellow.Value);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, darkYellow.Value);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, darkYellow.Value);
        ImGui::Button(buttonString.c_str());
    }

    ImGui::PopStyleColor(3);
}

void StatusBar::drawStatusBar(const InfoCache& infoCache) noexcept
{
    drawPrettyStatus("Item", infoCache.getItemQueueSize(), itemCount);
    ImGui::SameLine();
    drawPrettyStatus("TP", infoCache.getTpQueueSize(), tpCount);
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
