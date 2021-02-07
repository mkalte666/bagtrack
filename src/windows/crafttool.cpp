// licence note at the end of the file

#include "crafttool.h"
#include <imgui.h>
#include <imgui_fmt.h>

CraftTool::CraftTool() noexcept
    : Window("Craft Tool")
{
}

void CraftTool::update(Settings&, ItemTracker&, InfoCache&) noexcept
{
    if (!shown) {
        return;
    }

    ImGui::Begin(name.c_str(), &shown);

    ImGui::End();
}

void CraftTool::drawMainMenu() noexcept
{
    if (ImGui::BeginMenu("Items")) {
        if (ImGui::MenuItem("Crafting")) {
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
