// licence note at the end of the file
#include "sortitems.h"

void sortName(std::vector<ItemId>& itemIds, InfoCache& cache)
{
    std::sort(itemIds.begin(), itemIds.end(), [&cache](ItemId a, ItemId b) {
        return cache.getItemInfo(a).name.compare(cache.getItemInfo(b).name);
    });
}

std::vector<ItemId> sortItems(const ItemIdMap& items, ItemTracker& tracker, InfoCache& cache, const SortStrategy& sortStrategy) noexcept
{
    std::vector<ItemId> itemIds;
    for (const auto& pair : items) {
        itemIds.push_back(pair.first);
    }

    switch (sortStrategy) {
    case SortStrategy::SortNone:
        break;
    case SortStrategy::SortName:
        sortName(itemIds, cache);
        break;
    case SortStrategy::SortStrategyCount:
        break;
    }

    return itemIds;
}

std::vector<std::string> getSortStrategies() noexcept
{
    std::vector<std::string> strats;
    for (size_t i = 0; i < static_cast<size_t>(SortStrategy::SortStrategyCount); ++i) {
        auto strat = static_cast<SortStrategy>(i);
        switch (strat) {
        case SortStrategy::SortNone:
            strats.emplace_back("Dont Sort");
            break;
        case SortStrategy::SortName:
            strats.emplace_back("By Name");
            break;
        case SortStrategy::SortStrategyCount:
            break;
        }
    }

    return strats;
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
