// licence note at the end of the file

#include "updatechecker.h"
#include "../fixfmt.h"
#include "../imgui_fmt.h"
#include <SDL.h>
#include <chrono>
#include <cstdlib>
#include "../fixhttplib.h"
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <nlohmann/json.hpp>
#include <regex>

static constexpr const char* VersionCheckServer = "https://api.github.com";
static constexpr const char* VersionCheckEndpoint = "/repos/mkalte666/bagtrack/releases/latest";
static constexpr const char* VersionCheckDownloadURL = "https://github.com/mkalte666/bagtrack/releases/latest";

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

/**
 * \brief Convert a string to a version tag
 * \param tag a string in the format vMajor.Minor.Patch
 * \return Version Tag or empty version tag
 */
VersionInfo parseTag(const std::string& tag)
{
    // release tags always start with v, and then number dot number dot number
    static std::regex VersionRegex(R"(v(\d+)\.(\d+)\.(\d+))");
    std::smatch match;
    if (!std::regex_match(tag, match, VersionRegex) || match.size() != 4) {
        printDebug("Error Parsing release tag: Invalid format. Tag was '{}'", tag);
        return VersionInfo();
    }

    VersionInfo info;
    info.major = std::stoi(match[1]);
    info.minor = std::stoi(match[2]);
    info.patch = std::stoi(match[3]);

    return info;
}

UpdateChecker::UpdateChecker() noexcept
    : Window("Update Checker")
{
    releaseInfoResult = std::async(std::launch::async, [] {
        ReleaseInfo info;

        httplib::Client cli(VersionCheckServer);
        auto result = cli.Get(VersionCheckEndpoint);
        if (!result || result->status != 200) {
            return info;
        }

        try {
            auto j = nlohmann::json::parse(result->body);
            std::string tag = j.value("tag_name", "");
            info.version = parseTag(tag);
            info.title = j.value("name", "");
            info.text = j.value("body", "");
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

    if (releaseInfoResult.valid() && releaseInfoResult.wait_for(0ms) == std::future_status::ready) {
        upstreamInfo = releaseInfoResult.get();
    }

    if (BagtrackVersion < upstreamInfo.version) {
        ImGui::Begin("Update Available!", &shown, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::TextWrappedFmt("A New Version (\"{}\") is available!\nPress the button below to go to the download.", upstreamInfo.title);
        if (ImGui::CollapsingHeader("Show Release Notes")) {
            ImGui::TreePush();
            ImGui::TextWrappedFmt("{}", upstreamInfo.text);
            ImGui::TreePop();
        }
        if (ImGui::Button("Go to Download")) {
            openUrl(VersionCheckDownloadURL);
        }
        ImGui::TextWrappedFmt("Alternatively you can copy it below");
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
