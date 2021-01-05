// licence note at the end of the file

#ifndef BAGTRACK_ITEMTRACKER_H
#define BAGTRACK_ITEMTRACKER_H

#include "api/api.h"
#include "time_killer.h"
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>

#include "trackerstate.h"

/**
 * \brief Tracks current account inventory and delta between them
 */
class ItemTracker {
public:
    explicit ItemTracker(const Settings& settings) noexcept;
    ~ItemTracker() noexcept;
    ItemTracker(ItemTracker&&) = delete;
    ItemTracker(ItemTracker const&) = delete;
    ItemTracker& operator=(ItemTracker&&) = delete;
    ItemTracker& operator=(ItemTracker const&) = delete;

    /**
     * \brief Get the current (as in right now( inventory state
     * \return
     */
    TrackerState getCurrentState() const noexcept;
    /**
     * \brief Get a specific state
     * \param id
     * \return
     */
    TrackerState getState(int64_t id) const noexcept;
    /**
     * \brief Create a delta state (changes between states) from two states identified by ids
     * \param oldId
     * \param newId
     * \param keepList A list of items that should be kept in the list even if their delta is 0
     * \return
     */
    TrackerState getDeltaState(int64_t oldId, int64_t newId, const std::set<ItemId>& keepList = {}) const noexcept;
    /**
     * \brief Return the ID of the current (as in right now) inventory state
     * \return
     */
    int64_t getCurrentStateId() const noexcept;
    /**
     * \brief Return a list of all item state ids that are currently in the tracker
     * \return
     */
    std::set<int64_t> getStateIds() const noexcept;

    /**
     * \brief Get per-state changes for the item with the given id.
     * \param id
     * \param start
     * \param end
     * \return a map in the format [stateId: itemDelta, stateId2: itemDelta2, ...]
     */
    std::map<int64_t, int64_t> getItemStats(ItemId id, int64_t start, int64_t end) const noexcept;

    /**
     * \brief Cleanup all old states and flush the tracker cache
     */
    void clearHistory() noexcept;
    /**
     * \brief write the current tracker state cache to disk
     * \param filename
     */
    void writeCache(const fs::path& filename) const noexcept;
    /**
     * \brief read a tracker state cache from disk
     * \param filename
     */
    void readCache(const fs::path& filename) noexcept;

private:
    /**
     * \brief thread loop that gets stuff from the api
     * \param settings
     */
    void updateFunc(const Settings& settings) noexcept;
    /**
     * \brief get the state id for right now
     * \return
     */
    static int64_t stateIdFromCurrentTime() noexcept;

    /**
     * \brief Collect items from multiple api endpoints and merge them
     * \param apiKey
     * \return
     */
    static ItemIdMap collectAllItemSources(const std::string& apiKey) noexcept;
    std::thread updateThread = {};
    std::map<int64_t, TrackerState> states = {};
    mutable std::mutex mutex = {};
    timer_killer killer = {};
};

#endif //BAGTRACK_ITEMTRACKER_H
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