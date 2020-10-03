// licence note at the end of the file

#ifndef BAGTRACK_INFOCACHE_H
#define BAGTRACK_INFOCACHE_H

#include "api.h"
#include "time_killer.h"
#include <set>
#include <thread>

class InfoCache {
public:
    InfoCache() noexcept;
    ~InfoCache() noexcept;
    InfoCache(const InfoCache&) = delete;
    InfoCache(InfoCache&&) = delete;
    InfoCache& operator=(const InfoCache&) = delete;
    InfoCache& operator=(InfoCache&&) = delete;

    const ItemInfo& getItemInfo(ItemId id) noexcept;
    const TpInfo& getTpInfo(ItemId id) noexcept;

private:
    void threadFun();
    void readInfoCache();
    void writeInfoCache() const;
    std::thread fetchThread = {};
    mutable std::mutex mutex = {};
    timer_killer killer = {};
    bool triedLoadingItemCache = false;
    ItemInfoMap itemInfoCache = {};
    std::set<ItemId> itemsToCache = {};
    TpInfoMap tpInfoCache = {};
    std::set<ItemId> tpInfosToCache = {};
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