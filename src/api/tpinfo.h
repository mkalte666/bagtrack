// licence note at the end of the file

#ifndef BAGTRACK_TPINFO_H
#define BAGTRACK_TPINFO_H

#include "iteminfo.h"
#include <chrono>

/**
 * \brief Trading post information from the api
 */
struct TpInfo {
    ItemId id = {}; /// the item id for this entry
    int64_t buyValue = 0; /// highest buy order
    int64_t sellValue = 0; /// lowest sell order
    std::chrono::steady_clock::time_point age = {}; ///
};

/// Map itemIds to tp infos
using TpInfoMap = std::map<ItemId, TpInfo>;

/**
 * \brief pretty print a gold value (XXYYZZ => xxG yyS zzC)
 * \param value
 * \return
 */
std::string prettyGoldValue(int64_t value) noexcept;

/// json convert for TpInfo
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