// licence note at the end of the file

#include "updatechecker.h"
#include "../fixfmt.h"
#include <SDL.h>
#include <chrono>
#include <cstdlib>
#include <httplib.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <nlohmann/json.hpp>

static constexpr const char* VersionCheckServer = "https://mkalte.me";
static constexpr const char* VersionCheckEndpoint = "/share/bagtrack/version.json";
static constexpr const char* VersionCheckDownloadURL = "https://mkalte.me/share/bagtrack/current/";

#ifdef WIN32
static constexpr const char* OpenCommand = "start";
#else
static constexpr const char* OpenCommand = "xdg-open";
#endif

void openUrl(const std::string& url)
{
    std::string command = OpenCommand;
    command += " " + url;
    system(command.c_str()); // NOLINT find a better alternative at some point FIXME
}

UpdateChecker::UpdateChecker() noexcept
    : Window("Update Checker")
{
    versionInfoResult = std::async(std::launch::async, [] {
        VersionInfo info;

        httplib::Client cli(VersionCheckServer);
        auto result = cli.Get(VersionCheckEndpoint);
        if (!result || result->status != 200) {
            return info;
        }

        try {
            auto j = nlohmann::json::parse(result->body);
            info.major = j.value("major", 0);
            info.minor = j.value("minor", 0);
            info.patch = j.value("patch", 0);
        } catch (const nlohmann::json::exception& e) {
            printDebug("Error Parsing Version Infos: {}", e.what());
        }
        return info;
    });
    shown = true;
}

void UpdateChecker::update(Settings&, ItemTracker&, InfoCache&) noexcept
{
    if (!shown) {
        return;
    }
    using namespace std::chrono_literals;

    if (versionInfoResult.valid() && versionInfoResult.wait_for(0ms) == std::future_status::ready) {
        upstreamVersion = versionInfoResult.get();
    }

    if (BagtrackVersion < upstreamVersion) {
        ImGui::Begin("Update Available!", &shown, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::TextWrapped("A New Version is available!\nPress the button below to go to the download.");
        if (ImGui::Button("Go to Download")) {
            openUrl(VersionCheckDownloadURL);
        }
        ImGui::TextWrapped("Alternatively you can copy it below");
        std::string url = VersionCheckDownloadURL;
        ImGui::InputText("##dlUrl", &url, ImGuiInputTextFlags_ReadOnly);
        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::Selectable("Copy")) {
                SDL_SetClipboardText(VersionCheckDownloadURL);
            }
            ImGui::EndPopup();
        }

        ImGui::End();
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
