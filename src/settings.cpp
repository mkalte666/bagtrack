// licence note at the end of the file

#include "settings.h"
#include "messagebox.h"
#include <SDL.h>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

fs::path Settings::getPrefPath() noexcept
{
    char* prefPtr = SDL_GetPrefPath("mkalte", "bagtrack");
    fs::path prefPath = std::string(prefPtr);
    SDL_free(prefPtr);

    return prefPath;
}

Settings::Settings()
{
    read();
}

fs::path Settings::settingsFileName() noexcept
{
    fs::path name = getPrefPath();
    name /= "settings.json";
    return name;
}

std::string Settings::getApiKey() const noexcept
{
    return apiKey;
}

void Settings::setApiKey(std::string key) noexcept
{
    apiKey = key;
    write();
}

const std::set<ItemId>& Settings::getTrackedItems() const noexcept
{
    return trackedItems;
}

void Settings::setTrackedItems(const std::set<ItemId>& items) noexcept
{
    trackedItems = items;
    write();
}

void Settings::read()
{
    std::ifstream i(settingsFileName());
    if (!i.is_open()) {
        write();
    }

    i = std::ifstream(settingsFileName());
    if (!i.is_open()) {
        messageBox(SDL_MESSAGEBOX_ERROR, "Settings file", "Cannot read settings file!");
        std::terminate();
    }
    try {
        json j;
        i >> j;
        apiKey = j.value("apikey", DefaultApiKey);
        trackedItems = j.value("trackedItems", std::set<ItemId>());
        lastHistoryFile = j.value("lastHistoryFile", "");
        windowShown = j.value("windowShown", std::map<std::string, bool>());
    } catch (const json::exception& e) {
        printDebug("cannot read settings file! Defaulting values.\n{}\n", e.what());
        apiKey = DefaultApiKey;
        trackedItems = std::set<ItemId>();
        lastHistoryFile = getPrefPath() / DefaultHistoryFileName;
        windowShown = std::map<std::string, bool>();
    }
}

void Settings::write() const
{
    json j;
    j["apikey"] = apiKey;
    j["trackedItems"] = trackedItems;
    j["lastHistoryFile"] = lastHistoryFile.string();
    j["windowShown"] = windowShown;

    std::ofstream o(settingsFileName());
    if (!o.good()) {
        messageBox(SDL_MESSAGEBOX_ERROR, "Settings file", "Cannot read or write settings file!");
        std::terminate();
    }

    o << std::setw(4) << j << std::endl;
}

fs::path Settings::getLastHistoryFile() const noexcept
{
    if (lastHistoryFile.empty()) {
        return getPrefPath() / DefaultHistoryFileName;
    }

    return lastHistoryFile;
}

void Settings::setLastHistoryFile(const fs::path& filename) noexcept
{
    lastHistoryFile = filename;
    write();
}

bool Settings::getWindowShown(const std::string& windowName) const noexcept
{
    if (const auto iter = windowShown.find(windowName); iter != windowShown.end()) {
        return iter->second;
    }

    return false;
}

void Settings::setWindowShown(const std::string& windowName, bool shown) noexcept
{
    windowShown[windowName] = shown;
    write();
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