// licence note at the end of the file

#include "itemtracker.h"
#include "fixfmt.h"
#include <ctime>
#include <fstream>

using namespace std::chrono_literals;

ItemTracker::ItemTracker(const Settings& settings) noexcept
{
    updateThread = std::thread([this, &settings]() { this->updateFunc(settings); });
}
ItemTracker::~ItemTracker() noexcept
{
    killer.kill();
    updateThread.join();
}

void ItemTracker::updateFunc(const Settings& settings) noexcept
{
    do {
        if (states.empty()) {
            readCache(settings.getLastHistoryFile());
        }

        auto apiKey = settings.getApiKey();
        if (!checkApiKey(apiKey)) {
            fmt::print(stderr, "Api Key Invalid, tracker in pause");
            continue;
        }

        TrackerState newState;
        newState.stateId = stateIdFromCurrentTime();
        newState.coins = getAccountCoins(apiKey);
        newState.items = collectAllItemSources(apiKey);
        // error somewhere, or actually no items? move along
        if (newState.items.empty()) {
            continue;
        }

        {
            std::unique_lock lock(mutex);
            states[newState.stateId] = newState;
        }

        writeCache(settings.getLastHistoryFile());
    } while (killer.wait_for(60s));
}

ItemIdMap ItemTracker::collectAllItemSources(const std::string& apiKey) noexcept
{
    auto mergeMap = [](ItemIdMap& dst, const ItemIdMap& src) {
        for (const auto& pair : src) {
            dst[pair.first] += pair.second;
        }
    };

    auto maybeMaterials = getMaterialStorageContents(apiKey);
    auto maybeBank = getBankContents(apiKey);
    auto maybeChars = getCharacterContents(apiKey);
    if (!maybeMaterials.has_value() || !maybeBank.has_value() || !maybeChars.has_value()) {
        return ItemIdMap();
    }

    ItemIdMap items = maybeMaterials.value();
    mergeMap(items, maybeBank.value());
    mergeMap(items, maybeChars.value());
    return items;
}

TrackerState ItemTracker::getCurrentState() const noexcept
{
    return getState(getCurrentStateId());
}

TrackerState ItemTracker::getState(int64_t id) const noexcept
{
    std::lock_guard lockGuard(mutex);
    TrackerState copy = {};
    if (const auto iter = states.find(id); iter != states.end()) {
        copy = iter->second;
    }

    return copy;
}

TrackerState ItemTracker::getDeltaState(int64_t oldId, int64_t newId) const noexcept
{
    TrackerState delta = getState(newId);
    TrackerState oldState = getState(oldId);
    // no new state or no old state? no delta!
    if (delta.items.empty()) {
        return TrackerState();
    }
    if (oldState.items.empty()) {
        return TrackerState();
    }

    // calculate the item delta, dropping empty rows
    for (const auto& pair : oldState.items) {
        delta.items[pair.first] -= pair.second;
        if (delta.items[pair.first] == 0) {
            delta.items.erase(pair.first);
        }
    }

    // calculate the gold delta
    delta.coins -= oldState.coins;

    return delta;
}

int64_t ItemTracker::getCurrentStateId() const noexcept
{
    auto id = stateIdFromCurrentTime();
    if (states.count(id) == 0) {
        return id - 1;
    }

    return id;
}

std::set<int64_t> ItemTracker::getStateIds() const noexcept
{
    std::lock_guard lockGuard(mutex);
    std::set<int64_t> ids;
    for (const auto& pair : states) {
        ids.emplace(pair.first);
    }

    return ids;
}

void ItemTracker::writeCache(const fs::path& filename) const noexcept
{
    std::map<int64_t, TrackerState> copy;
    {
        std::unique_lock lock(mutex);
        copy = states;
    }

    nlohmann::json j;
    nlohmann::to_json(j, states);
    std::ofstream file(filename);
    if (file.good()) {
        file << j;
    }
}

void ItemTracker::readCache(const fs::path& filename) noexcept
{
    nlohmann::json j;
    std::ifstream file(filename);
    if (file.good()) {
        try {
            file >> j;
            std::unique_lock lock(mutex);
            nlohmann::from_json(j, states);
        } catch (nlohmann::json::exception& e) {
            fmt::print(stderr, "Cannot read json history! {}", e.what());
            // valid state, invalid file, we dont care
        }
    }
}

int64_t ItemTracker::stateIdFromCurrentTime() noexcept
{
    auto id = static_cast<int64_t>(std::time(nullptr));
    id /= 300; // divide by 300 to make one unit equal 5 minutes
    return id;
}

void ItemTracker::clearHistory() noexcept
{
    std::lock_guard lockGuard(mutex);
    states.clear();
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
