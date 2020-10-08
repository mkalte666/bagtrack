// licence note at the end of the file
#ifndef REMUTE_SETTINGS_H
#define REMUTE_SETTINGS_H

#include <filesystem>
#include <map>
#include <set>
#include <string>
namespace fs = std::filesystem;

#include "iteminfo.h"

/**
 * \brief Settings persisting across sessions
 */
class Settings {
public:
    static constexpr const char* DefaultApiKey = "";
    static constexpr const char* DefaultHistoryFileName = "history.bagtrack";

    /**
     * \brief Equivalent to calling SDL_GetPrefPath("mkalte", "bagtrack")
     * \return
     */
    static fs::path getPrefPath() noexcept;

    /**
     * \brief CTor. Loads settings from disk or creates a new file
     */
    Settings();

    ~Settings() = default;
    Settings(const Settings&) = delete;
    Settings(Settings&&) = default;
    Settings& operator=(const Settings&) = delete;
    Settings& operator=(Settings&&) = default;

    /**
     * \brief Get the gw2 api key. generated on account.arena.net
     * \return
     */
    [[nodiscard]] std::string getApiKey() const noexcept;
    /**
     * \brief Set the gw2 api key. generated on account.arena.net
     * \param key
     */
    void setApiKey(std::string key) noexcept;

    [[nodiscard]] fs::path getLastHistoryFile() const noexcept;
    void setLastHistoryFile(const fs::path& filename) noexcept;

    /**
     * \brief Return list of currently tracked items
     * \return
     */
    [[nodiscard]] const std::set<ItemId>& getTrackedItems() const noexcept;

    /**
     * \brief Set the list of currently tracked items
     */
    void setTrackedItems(const std::set<ItemId>& items) noexcept;

    /**
     * \brief Check if window with the name is shown
     * \param windowName
     * \return
     */
    [[nodiscard]] bool getWindowShown(const std::string& windowName) const noexcept;

    /**
     * \brief Save if with the window with the name is currently shown
     * \param windowName
     * \param shown
     */
    void setWindowShown(const std::string& windowName, bool shown) noexcept;

private:
    static fs::path settingsFileName() noexcept;
    void read();
    void write() const;
    std::string apiKey = DefaultApiKey;
    fs::path lastHistoryFile = {};
    std::set<ItemId> trackedItems = {};
    std::map<std::string, bool> windowShown = {};
};

#endif //REMUTE_SETTINGS_H
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