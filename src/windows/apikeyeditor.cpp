// licence note at the end of the file
#include "apikeyeditor.h"
#include "../api.h"
#include "../sdlhelpers.h"
#include <cstdio>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

ApiKeyEditor::ApiKeyEditor()
    : Window("Api Key Editor")
{
}

void ApiKeyEditor::update(Settings& settings, ItemTracker&, InfoCache&) noexcept
{
    // force show the window if there is no key set. Only apply if the key is checked
    if (apiKey.empty()) {
        apiKey = settings.getApiKey();
    }

    if (apiKey == Settings::DefaultApiKey) {
        shown = true;
        keyChecked = false;
        ImGui::SetNextWindowFocus();
    }

    // everything else is unimportant if we are not shown
    if (!shown) {
        return;
    }

    ImGui::Begin(name.c_str(), &shown);
    ImGui::BeginChild("contents", ImVec2(0, -50.0F));

    ImGui::SetNextItemWidth(120.0F);
    if (ImGui::InputText("GW2 Api Key", &apiKey, ImGuiInputTextFlags_Password)) {
        keyChecked = false;
    }
    if (ImGui::BeginPopupContextItem()) {
        if (ImGui::Selectable("Paste")) {
            keyChecked = false;
            apiKey = getClipboard();
        }
        ImGui::EndPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button("Check Key")) {
        keyChecked = checkApiKey(apiKey);
    }

    ImGui::TextWrapped("You need 'account', 'inventories' and 'wallet' enabled for this key!"); // NOLINT
    ImGui::EndChild();

    if (ImGui::Button("Cancel")) {
        apiKey = settings.getApiKey();
        shown = false;
    }

    ImGui::SameLine();
    if (keyChecked) {
        if (ImGui::Button("Save")) {
            settings.setApiKey(apiKey);
            shown = false;
            keyChecked = false;
        }
    } else {
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
        ImGui::Button("Save");
        ImGui::PopStyleVar();
    }

    ImGui::End();
}

void ApiKeyEditor::drawMainMenu() noexcept
{

    if (ImGui::BeginMenu("GW2")) {
        ImGui::Separator();
        if (ImGui::Selectable("Api Key...")) {
            shown = true;
        }
        ImGui::Separator();
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
