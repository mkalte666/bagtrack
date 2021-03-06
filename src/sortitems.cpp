// licence note at the end of the file
#include "sortitems.h"

void sortName(std::vector<ItemId>& itemIds, InfoCache& cache)
{
    std::sort(itemIds.begin(), itemIds.end(), [&cache](ItemId a, ItemId b) {
        const std::string aName = cache.getItemInfo(a).name;
        const std::string bName = cache.getItemInfo(b).name;
        return aName < bName;
    });
}

void sortCount(std::vector<ItemId>& itemIds, const ItemIdMap& items)
{
    std::sort(itemIds.begin(), itemIds.end(), [&items](ItemId a, ItemId b) {
        const auto iterA = items.find(a);
        const auto iterB = items.find(b);
        if (iterA == items.end() || iterB == items.end()) {
            return false;
        }
        return iterA->second() > iterB->second();
    });
}

void sortSellValue(std::vector<ItemId>& itemIds, InfoCache& cache)
{
    std::sort(itemIds.begin(), itemIds.end(), [&cache](ItemId a, ItemId b) {
        return cache.getTpInfo(a).sellValue > cache.getTpInfo(b).sellValue;
    });
}

void sortBuyValue(std::vector<ItemId>& itemIds, InfoCache& cache)
{
    std::sort(itemIds.begin(), itemIds.end(), [&cache](ItemId a, ItemId b) {
        return cache.getTpInfo(a).buyValue > cache.getTpInfo(b).buyValue;
    });
}

std::vector<ItemId> sortItems(const ItemIdMap& items, InfoCache& cache, const SortStrategy& sortStrategy) noexcept
{
    std::vector<ItemId> itemIds;
    for (const auto& pair : items) {
        itemIds.push_back(pair.first);
    }

    switch (sortStrategy) {
    case SortStrategy::None:
        break;
    case SortStrategy::Name:
        sortName(itemIds, cache);
        break;
    case SortStrategy::Count:
        sortCount(itemIds, items);
        break;
    case SortStrategy::SellValue:
        sortBuyValue(itemIds, cache);
        break;
    case SortStrategy::BuyValue:
        sortSellValue(itemIds, cache);
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
        case SortStrategy::None:
            strats.emplace_back("Dont Sort");
            break;
        case SortStrategy::Name:
            strats.emplace_back("By Name");
            break;
        case SortStrategy::Count:
            strats.emplace_back("By Count");
            break;
        case SortStrategy::SellValue:
            strats.emplace_back("By Sell Value");
            break;
        case SortStrategy::BuyValue:
            strats.emplace_back("By Buy Value");
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
