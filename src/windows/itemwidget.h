// licence note at the end of the file

#ifndef BAGTRACK_ITEMWIDGET_H
#define BAGTRACK_ITEMWIDGET_H

#include "../api/infocache.h"
#include "../api/iteminfo.h"
#include "../fixfmt.h"
#include "../sortitems.h"
#include <set>

struct ItemWidgetState {
    std::string filter = {};
    SortStrategy sortStrategy = SortStrategy::Name;
    std::vector<ItemId> sorted = {};
    std::vector<ItemId> filtered = {};
};

ItemId listItems(ItemWidgetState& state, const ItemIdMap& items, InfoCache& cache, size_t minFilterLetters = 0, const std::set<ItemId>& idFilter = std::set<ItemId>(), bool showInfos = true);

#endif //BAGTRACK_ITEMWIDGET_H
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
