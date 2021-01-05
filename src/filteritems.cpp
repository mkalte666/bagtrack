// licence note at the end of the file
#include "filteritems.h"

// toLower modies chars but this is not too nice on the compiler warnings
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#endif
std::string toLower(const std::string& in)
{
    std::string result = in;
    for (char& c : result) {
        if (c >= 'A' && c <= 'Z') {
            c += 32;
        }
    }
    return result;
}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

std::vector<ItemId> filterItems(const std::vector<ItemId>& items, InfoCache& cache, const std::string& filter) noexcept
{
    if (filter.empty()) {
        return items;
    }

    std::vector<ItemId> filteredItems;
    filteredItems.reserve(items.size());
    for (const auto& item : items) {
        const auto& info = cache.getItemInfo(item);
        if (toLower(info.name).find(toLower(filter)) != std::string::npos || toLower(info.description).find(toLower(filter)) != std::string::npos) {
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
