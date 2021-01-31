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
            printDebug("Api Key Invalid, tracker in pause");
            continue;
        }

        TrackerState newState;
        newState.stateId = stateIdFromCurrentTime();
        newState.coins = getAccountCoins(apiKey);
        newState.items = collectAllItemSources(apiKey);
        // error somewhere, or actually no items? move along
        if (newState.items.allItems.empty()) {
            continue;
        }

        {
            std::unique_lock lock(mutex);
            states[newState.stateId] = newState;
        }

        writeCache(settings.getLastHistoryFile());
    } while (killer.wait_for(60s));
}

TrackerState::ItemSourceResults ItemTracker::collectAllItemSources(const std::string& apiKey) noexcept
{
    auto merge = [](TrackerState::ItemSourceResults& dst, const ItemIdMap& src, std::string_view srcName) {
        dst.itemSources.emplace_back(srcName);
        for (const auto& pair : src) {
            dst.allItems[pair.first] += pair.second;
            dst.itemReverseLookup[pair.first][dst.itemSources.size() - 1] += pair.second;
        }
    };

    auto maybeMaterials = getMaterialStorageContents(apiKey);
    auto maybeBank = getBankContents(apiKey);
    auto maybeChars = getCharacterContents(apiKey);
    auto maybeShared = getSharedInventory(apiKey);
    if (!maybeMaterials.has_value() || !maybeBank.has_value() || !maybeChars.has_value() || !maybeShared.has_value()) {
        return TrackerState::ItemSourceResults();
    }

    TrackerState::ItemSourceResults results;
    merge(results, maybeMaterials.value(), "Material Storage");
    merge(results, maybeBank.value(), "Bank");
    merge(results, maybeShared.value(), "Shared Inventory");
    for (const auto& pair : maybeChars.value()) {
        merge(results, pair.second, pair.first);
    }
    return results;
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

TrackerState ItemTracker::getDeltaState(int64_t oldId, int64_t newId, const std::set<ItemId>& keepList) const noexcept
{
    TrackerState delta = getState(newId);
    TrackerState oldState = getState(oldId);
    // no new state or no old state? no delta!
    if (delta.items.allItems.empty()) {
        return TrackerState();
    }
    if (oldState.items.allItems.empty()) {
        return TrackerState();
    }

    // calculate the item delta, dropping empty rows
    for (const auto& pair : oldState.items.allItems) {
        delta.items.allItems[pair.first] -= pair.second;
        if (delta.items.allItems[pair.first] == 0 && keepList.find(pair.first) == keepList.end()) {
            delta.items.allItems.erase(pair.first);
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
            printDebug("Cannot read json history! {}", e.what());
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

std::map<int64_t, int64_t> ItemTracker::getItemStats(ItemId id, int64_t start, int64_t end) const noexcept
{
    std::map<int64_t, int64_t> results;
    std::lock_guard lockGuard(mutex);
    auto iterStart = states.lower_bound(start);
    auto iterEnd = states.lower_bound(end);
    // make sure the iterators make sense
    if (iterStart == states.end() || iterEnd == states.end()) {
        return results;
    }

    // we pretend we have every value in the range
    // so we need to save the last iter in case iter itself is invalid
    auto lastIter = iterStart;
    // including end - end is not end of the set!
    for (int64_t i = start; i <= end; ++i) {
        auto iter = states.find(i);
        // check the iter, and maybe use last iter to make sure we always have a value
        if (iter == states.end()) {
            iter = lastIter;
        }
        const auto& itemIter = iter->second.items.allItems.find(id);
        if (itemIter == iter->second.items.allItems.end()) {
            results[i] = 0;
        } else {
            results[i] = itemIter->second;
        }
        lastIter = iter;
    }

    return results;
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
