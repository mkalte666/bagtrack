// licence note at the end of the file

#ifndef BAGTRACK_TPINFO_H
#define BAGTRACK_TPINFO_H

#include "iteminfo.h"
#include <chrono>

struct TpInfo {
    ItemId id = {};
    uint64_t buyValue = 0;
    uint64_t sellValue = 0;
    std::chrono::steady_clock::time_point age = {};
};

using TpInfoMap = std::map<ItemId, TpInfo>;

std::string prettyGoldValue(uint64_t value) noexcept;
void from_json(const nlohmann::json& j, TpInfo& tpInfo);
#endif //BAGTRACK_TPINFO_H
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