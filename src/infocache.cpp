// licence note at the end of the file
#include "infocache.h"
#include "settings.h"
#include <fstream>

InfoCache::InfoCache() noexcept
{
    fetchThread = std::thread([this]() { this->threadFun(); });
}

InfoCache::~InfoCache() noexcept
{
    killer.kill();
    fetchThread.join();
}

void InfoCache::threadFun()
{
    using namespace std::chrono_literals;
    while (killer.wait_for(1s)) {
        std::unique_lock lock(mutex);
        if (!triedLoadingItemCache) {
            triedLoadingItemCache = true;
            readInfoCache();
        }
        // get all the new items
        if (!itemsToCache.empty()) {
            const auto toCacheCopy = itemsToCache;
            lock.unlock();
            const auto newItems = getItemInfos(toCacheCopy);
            lock.lock();
            for (const auto& pair : newItems) {
                itemsToCache.erase(pair.first);
                itemInfoCache[pair.first] = pair.second;
            }
            writeInfoCache();
        }

        // check for the trading post infos
        if (!tpInfosToCache.empty()) {
            const auto toCacheCopy = tpInfosToCache;
            lock.unlock();
            const auto newInfos = getItemTpInfos(toCacheCopy);
            lock.lock();
            for (const auto& pair : newInfos) {
                tpInfosToCache.erase(pair.first);
                tpInfoCache[pair.first] = pair.second;
            }
        }
    }
}

const ItemInfo& InfoCache::getItemInfo(ItemId id) noexcept
{
    static const ItemInfo fallbackInfo = {};

    std::lock_guard lockGuard(mutex);
    if (const auto iter = itemInfoCache.find(id); iter != itemInfoCache.end()) {
        return iter->second;
    }

    itemsToCache.emplace(id);
    return fallbackInfo;
}

const TpInfo& InfoCache::getTpInfo(ItemId id) noexcept
{
    static const TpInfo fallbackInfo = {};
    using namespace std::chrono_literals;

    std::lock_guard lockGuard(mutex);

    // check if we have infos on the item
    if (const auto itemInfoIter = itemInfoCache.find(id); itemInfoIter != itemInfoCache.end()) {
        // ok we know it. make sure it has no binding
        if (itemInfoIter->second.checkIfBound()) {
            return fallbackInfo;
        }

        if (const auto iter = tpInfoCache.find(id); iter != tpInfoCache.end()) {
            const TpInfo& info = iter->second;
            if (std::chrono::steady_clock::now() - info.age > 60s) {
                tpInfosToCache.emplace(id);
            }
            return info;
        }

        tpInfosToCache.emplace(id);
    }

    // we dont know the item yet so do not fetch it
    return fallbackInfo;
}

void InfoCache::readInfoCache()
{
    auto cacheFileName = Settings::getPrefPath();
    cacheFileName /= "itemInfoCache.json";
    std::ifstream cacheFile(cacheFileName);
    if (cacheFile.is_open()) {
        nlohmann::json j;
        cacheFile >> j;
        nlohmann::from_json(j, itemInfoCache);
    }
}

void InfoCache::writeInfoCache() const
{
    auto cacheFileName = Settings::getPrefPath();
    cacheFileName /= "itemInfoCache.json";
    std::ofstream cacheFile(cacheFileName);
    if (cacheFile.good()) {
        nlohmann::json j;
        nlohmann::to_json(j, itemInfoCache);
        cacheFile << j;
    }
}

uint64_t InfoCache::estimateItemValue(ItemId id) noexcept
{
    const auto& info = getItemInfo(id);
    if (info.checkIfBound()) {
        return info.vendorValue;
    }

    const auto& tpInfo = getTpInfo(id);
    return tpInfo.sellValue;
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
