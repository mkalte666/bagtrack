// licence note at the end of the file
#include "filteritems.h"
#include <algorithm>

std::vector<ItemId> filterItems(const std::vector<ItemId>& items, InfoCache& cache, const std::string& filter) noexcept
{
    if (filter.empty()) {
        return items;
    }
    ItemIdList sortedInput = items;
    std::sort(sortedInput.begin(), sortedInput.end());
    ItemIdList matchingItems = cache.findItems(filter);
    std::sort(matchingItems.begin(), matchingItems.end());
    std::vector<ItemId> filteredItems;
    filteredItems.reserve(items.size());
    std::set_intersection(sortedInput.begin(), sortedInput.end(), matchingItems.begin(), matchingItems.end(), std::back_inserter(filteredItems));

    return filteredItems;
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
