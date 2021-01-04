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

class ItemTracker {
public:
    ItemTracker(const Settings& settings) noexcept;
    ~ItemTracker() noexcept;
    ItemTracker(ItemTracker&&) = delete;
    ItemTracker(ItemTracker const&) = delete;
    ItemTracker& operator=(ItemTracker&&) = delete;
    ItemTracker& operator=(ItemTracker const&) = delete;

    TrackerState getCurrentState() const noexcept;
    TrackerState getState(int64_t id) const noexcept;
    TrackerState getDeltaState(int64_t oldId, int64_t newId, const std::set<ItemId>& keepList = {}) const noexcept;
    int64_t getCurrentStateId() const noexcept;
    std::set<int64_t> getStateIds() const noexcept;

    std::map<int64_t, int64_t> getItemStats(ItemId id, int64_t start, int64_t end) const noexcept;

    void clearHistory() noexcept;
    void writeCache(const fs::path& filename) const noexcept;
    void readCache(const fs::path& filename) noexcept;

private:
    void updateFunc(const Settings& settings) noexcept;
    static int64_t stateIdFromCurrentTime() noexcept;

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