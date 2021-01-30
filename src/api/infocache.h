// licence note at the end of the file

#ifndef BAGTRACK_INFOCACHE_H
#define BAGTRACK_INFOCACHE_H

#include "../time_killer.h"
#include "api.h"
#include "index.h"
#include <set>
#include <thread>

/**
 * \brief The InfoCache caches static (non-changing) item information
 */
class InfoCache {
public:
    explicit InfoCache() noexcept;
    ~InfoCache() noexcept;
    InfoCache(const InfoCache&) = delete;
    InfoCache(InfoCache&&) = delete;
    InfoCache& operator=(const InfoCache&) = delete;
    InfoCache& operator=(InfoCache&&) = delete;

    /**
     * \brief Find all items that match s
     * \param s
     * \return
     */
    ItemIdList findItems(std::string_view s) const noexcept;

    /**
     * \brief Get the static item information for the id
     * \param id
     * \return
     */
    const ItemInfo& getItemInfo(ItemId id) noexcept;
    /**
     * \brief Get the current trading post info for the id
     * \param id
     * \return
     */
    const TpInfo& getTpInfo(ItemId id) noexcept;
    /**
     * \brief Estimate the value of item id
     * \param id
     * \return
     */
    int64_t estimateItemValue(ItemId id) noexcept;

    /**
     * \brief Clean up the item info cahce
     */
    void clearCache() noexcept;

    /**
     * \brief Cache all items.
     */
    void cacheAll() noexcept;

    /**
     * \brief Gets the size of itemsToCache
     * \return
     */
    size_t getItemQueueSize() const noexcept;

    /**
     * \brief Gets the size of tpInfosToCache
     * \return
     */
    size_t getTpQueueSize() const noexcept;

private:
    /// thread worker
    void threadFun();
    /// read cache from the disk
    void readInfoCache();
    /// write cache to disk
    void writeInfoCache() const;
    std::thread fetchThread = {};
    mutable std::mutex mutex = {};
    timer_killer killer = {};
    bool triedLoadingItemCache = false;
    ItemInfoMap itemInfoCache = {};
    ItemIdList itemsToCache = {};
    bool fillLongList = false;
    ItemIdList itemsLongList = {};
    TpInfoMap tpInfoCache = {};
    ItemIdList tpInfosToCache = {};
    Index searchIndex = {};
};

#endif //BAGTRACK_INFOCACHE_H
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