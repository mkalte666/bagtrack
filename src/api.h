// licence note at the end of the file

#ifndef BAGTRACK_API_H
#define BAGTRACK_API_H

#include "iteminfo.h"
#include "settings.h"
#include <map>
#include <set>

using ItemIdMap = std::map<ItemId, uint64_t>;
using ItemInfoMap = std::map<ItemId, ItemInfo>;

bool checkApiKey(const std::string& key) noexcept;

ItemIdMap getBankContents(const std::string& key) noexcept;
ItemIdMap getMaterialStorageContents(const std::string& key) noexcept;
//ItemIdMap getCharacterContents(const std::string& key) noexcept;

ItemInfoMap getItemInfos(const std::set<ItemId>& ids) noexcept;

#endif //BAGTRACK_API_H
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