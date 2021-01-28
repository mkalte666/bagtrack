// licence note at the end of the file
#include "infocache.h"
#include "../settings.h"
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
    while (killer.wait_for(100ms)) {
        std::unique_lock lock(mutex);
        if (!triedLoadingItemCache) {
            triedLoadingItemCache = true;
            readInfoCache();
        }
        // get all the new items
        if (fillLongList) {
            itemsToCache = getAllItemIds(); // all items is all items folks
            fillLongList = false;
        }
        if (!itemsToCache.empty()) {
            auto toCacheCopyBeforeFilter = itemsToCache;
            lock.unlock();
            // this is slow, but fetching things twice is a lot slower
            ItemIdList toCacheCopy;
            toCacheCopy.reserve(toCacheCopyBeforeFilter.size());
            for (const auto& item : toCacheCopyBeforeFilter) {
                lock.lock();
                if (itemInfoCache.find(item) == itemInfoCache.end()) {
                    toCacheCopy.push_back(item);
                }
                lock.unlock();
            }
            const auto newItems = getItemInfos(toCacheCopy);
            lock.lock();
            for (const auto& pair : newItems) {
                itemInfoCache[pair.first] = pair.second;
            }
            itemsToCache = std::move(toCacheCopy);
            writeInfoCache();
        }

        // check for the trading post infos
        // we dont check for duplicates here as tp data change!
        if (!tpInfosToCache.empty()) {
            auto toCacheCopy = tpInfosToCache;
            lock.unlock();
            const auto newInfos = getItemTpInfos(toCacheCopy);
            lock.lock();
            for (const auto& pair : newInfos) {
                tpInfoCache[pair.first] = pair.second;
            }
            tpInfosToCache = std::move(toCacheCopy);
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

    itemsToCache.push_back(id);
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
                tpInfosToCache.push_back(id);
            }
            return info;
        }

        tpInfosToCache.push_back(id);
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

int64_t InfoCache::estimateItemValue(ItemId id) noexcept
{
    const auto& info = getItemInfo(id);
    if (info.checkIfBound()) {
        return info.vendorValue;
    }

    const auto& tpInfo = getTpInfo(id);
    return tpInfo.sellValue;
}

void InfoCache::clearCache() noexcept
{
    std::lock_guard lockGuard(mutex);
    tpInfoCache.clear();
    itemInfoCache.clear();
    writeInfoCache();
}

void InfoCache::cacheAll() noexcept
{
    std::lock_guard lockGuard(mutex);
    fillLongList = true;
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
