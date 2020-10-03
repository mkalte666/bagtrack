// licence note at the end of the file
#include "filteritems.h"

std::vector<ItemId> filterItems(const std::vector<ItemId>& items, InfoCache& cache, const std::string& filter) noexcept
{
    if (filter.empty()) {
        return items;
    }

    std::vector<ItemId> filteredItems;
    filteredItems.reserve(items.size());
    for (const auto& item : items) {
        const auto& info = cache.getItemInfo(item);
        if (info.name.find(filter) != std::string::npos || info.description.find(filter) != std::string::npos) {
            filteredItems.push_back(item);
        }
    }

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
