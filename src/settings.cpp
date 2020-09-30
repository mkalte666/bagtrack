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

    json j;
    i >> j;
    apiKey = j.value("apikey", DefaultApiKey);
}

void Settings::write() const
{
    json j;
    j["apikey"] = apiKey;

    std::ofstream o(settingsFileName());
    if (!o.good()) {
        messageBox(SDL_MESSAGEBOX_ERROR, "Settings file", "Cannot read or write settings file!");
        std::terminate();
    }

    o << std::setw(4) << j << std::endl;
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