// licence note at the end of the file
#include "infocache.h"
#include <fstream>

InfoCache::InfoCache() noexcept
{
    fetchThread = std::thread([this]() { this->threadFun(); });
}

InfoCache::~InfoCache() noexcept
{
    killer.kill();
    fetchThread.join();
    writeInfoCache();
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
        // old format
        if (!j.is_object()) {
            nlohmann::from_json(j, itemInfoCache);
        }
        // new format
        else {
            itemInfoCache = j.value("itemInfos", ItemInfoMap());
            recipeCache = j.value("recipes", RecipeMap());
            recipeLookupCache = j.value("recipeLookupCache", std::map<ItemId, RecipeIdList>());
        }
    }
}

void InfoCache::writeInfoCache() const
{
    auto cacheFileName = Settings::getPrefPath();
    cacheFileName /= "itemInfoCache.json";
    std::ofstream cacheFile(cacheFileName);
    if (cacheFile.good()) {
        nlohmann::json j;
        j["itemInfos"] = itemInfoCache;
        j["recipes"] = recipeCache;
        j["recipeLookupCache"] = recipeLookupCache;
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
    recipeCache.clear();
    recipeLookupCache.clear();
    writeInfoCache();
}

void InfoCache::cacheAll() noexcept
{
    std::lock_guard lockGuard(mutex);
    fillLongList = true;
}

size_t InfoCache::getItemQueueSize() const noexcept
{
    std::lock_guard lockGuard(mutex);
    return itemsToCache.size();
}

size_t InfoCache::getTpQueueSize() const noexcept
{
    std::lock_guard lockGuard(mutex);
    return tpInfosToCache.size();
}

ItemIdList InfoCache::findItems(const std::string_view s) const noexcept
{
    std::lock_guard lockGuard(mutex);
    return searchIndex.find(s);
}

const RecipeIdList& InfoCache::findRecipesForItem(ItemId id) noexcept
{
    static const RecipeIdList fallbackList = {};
    std::lock_guard lockGuard(mutex);

    if (const auto& iter = recipeLookupCache.find(id); iter != recipeLookupCache.end()) {
        return iter->second;
    }

    recipeLookupsToCache.push_back(id);
    return fallbackList;
}

const Recipe& InfoCache::getRecipe(RecipeId id) noexcept
{
    static const Recipe fallbackRecipe = {};
    std::lock_guard lockGuard(mutex);

    if (const auto& iter = recipeCache.find(id); iter != recipeCache.end()) {
        return iter->second;
    }

    recipeToCache.push_back(id);
    return fallbackRecipe;
}

void InfoCache::threadFun()
{
    using namespace std::chrono_literals;
    while (killer.wait_for(100ms)) {
        // scope for lock
        {
            std::unique_lock lock(mutex);
            // loading
            if (!triedLoadingItemCache) {
                triedLoadingItemCache = true;
                readInfoCache();
            }

            // if the index is empty, we either just introduced it or its broked
            // so redownload everything! (yes, i know)
            if (!searchIndex.hasAnyData() && !itemInfoCache.empty()) {
                itemsToCache.reserve(itemInfoCache.size());
                for (const auto& pair : itemInfoCache) {
                    itemsToCache.push_back(pair.first);
                }
                itemInfoCache.clear(); //YES! broken index means we need to rebuild.
            }
            // get all the new items
            else if (fillLongList) {
                itemsToCache = getAllItemIds(); // all items is all items folks
                fillLongList = false;
            }
        }

        itemInfoWork();
        tpInfoWork();
        recipeWork();
        recipeLookupWork();
    }
}

void InfoCache::itemInfoWork() noexcept
{
    std::unique_lock lock(mutex);
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
        for (const auto& pair : newItems) {
            lock.lock();
            itemInfoCache[pair.first] = pair.second;
            searchIndex.addItem(pair.second);
            lock.unlock();
        }
        lock.lock();
        itemsToCache = std::move(toCacheCopy);
        writeInfoCache();
    }
}

void InfoCache::tpInfoWork() noexcept
{
    std::unique_lock lock(mutex);
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

void InfoCache::recipeWork() noexcept
{
    std::unique_lock lock(mutex);
    if (!recipeToCache.empty()) {
        RecipeIdList toCacheCopyNotFiltered = recipeToCache;
        lock.unlock();
        RecipeIdList toCacheCopy;
        toCacheCopy.reserve(toCacheCopyNotFiltered.size());
        // filter out duplicates
        for (const auto id : toCacheCopyNotFiltered) {
            if (const auto iter = recipeCache.find(id); iter != recipeCache.end()) {
                toCacheCopy.push_back(id);
            }
        }

        const auto newRecipes = getRecipes(toCacheCopy);
        lock.lock();
        recipeToCache = std::move(toCacheCopy);
        for (const auto& recipe : newRecipes) {
            recipeCache[recipe.id] = recipe;
        }
    }
}

void InfoCache::recipeLookupWork() noexcept
{
    // lookup is only for one item.
    // if the list builds up
    // we dont wanna take forever to wait for the api
    constexpr size_t maxRequestsPerCall = 5;
    std::unique_lock lock(mutex);
    if (!recipeToCache.empty()) {
        ItemIdList lookupListCopy = recipeLookupsToCache;
        lock.unlock();
        size_t count = 0;
        for (const auto lookupItemId : lookupListCopy) {
            // check for duplicates
            if (const auto iter = recipeLookupCache.find(lookupItemId); iter != recipeLookupCache.end()) {
                const auto newLookups = getRecipesForItem(lookupItemId);
                // insert needs locking
                lock.lock();
                recipeLookupCache[lookupItemId] = newLookups;
                lock.unlock();
            }
            ++count;
            if (count >= maxRequestsPerCall) {
                break;
            }
        }
        // remove count items from the tooLookup cache
        lock.lock();
        if (count >= recipeToCache.size()) {
            recipeToCache.clear();
        } else {
            recipeToCache = std::vector(recipeToCache.begin() + static_cast<ptrdiff_t>(count), recipeToCache.end());
        }
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
