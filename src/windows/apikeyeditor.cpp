// licence note at the end of the file
#include "apikeyeditor.h"
#include <cstdio>
#include <fmt/format.h>
#include <httplib.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

bool checkGw2ApiKey(const std::string& key) noexcept
{
    httplib::Client client("https://api.guildwars2.com");
    client.set_bearer_token_auth(key.c_str());
    auto res = client.Get("/v2/tokeninfo");
    if (res->status != 200 && res->status != 401) {
        fmt::print(stderr, "Token check failed - return code {}\n", res->status);
        return false;
    }

    auto keyInfo = json::parse(res->body);
    if (keyInfo.contains("text") && keyInfo["text"] == "Invalid access token") {
        fmt::print(stderr, "Token check failed - invalid token\n");
        return false;
    }

    if (!keyInfo.contains("permissions")) {
        fmt::print(stderr, "Token check failed - invalid token information\n");
        return false;
    }

    try {
        std::vector<std::string> permissions = keyInfo["permissions"];
        if (std::find(permissions.begin(), permissions.end(), "account") == permissions.end()
            || std::find(permissions.begin(), permissions.end(), "inventories") == permissions.end()
            || std::find(permissions.begin(), permissions.end(), "characters") == permissions.end()
            || std::find(permissions.begin(), permissions.end(), "wallet") == permissions.end()) {
            fmt::print(stderr, "Token is missing permissions\n");
            return false;
        }
    } catch (const json::exception& e) {
        fmt::print(stderr, "Token info parsing failed, {}\n", e.what());
        return false;
    }

    fmt::print(stderr, "Token check successfull\n");
    return true;
}

ApiKeyEditor::ApiKeyEditor()
    : Window("Api Key Editor")
{
}

void ApiKeyEditor::update(Settings& settings) noexcept
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
    ImGui::SameLine();
    if (ImGui::Button("Check Key")) {
        keyChecked = checkGw2ApiKey(apiKey);
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
        if (ImGui::Selectable("Api Key...")) {
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
