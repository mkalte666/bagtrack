// licence note at the end of the file

#ifndef BAGTRACK_SORTITEMS_H
#define BAGTRACK_SORTITEMS_H

#include "infocache.h"
#include "itemtracker.h"

enum class SortStrategy {
    SortNone,
    SortName,
    // edit before here, this one always the last!
    SortStrategyCount
};

/**
 * \brief Sort items by the given sorting strategy
 * \param items
 * \param tracker
 * \param cache
 * \param sortStrategy
 * \return
 */
std::vector<ItemId> sortItems(const ItemIdMap& items, ItemTracker& tracker, InfoCache& cache, const SortStrategy& sortStrategy) noexcept;

std::vector<std::string> getSortStrategies() noexcept;

#endif //BAGTRACK_SORTITEMS_H
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