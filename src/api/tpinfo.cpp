// Licence note is at the end of the file

#include "tpinfo.h"
#include "../fixfmt.h"

using json = nlohmann::json;

std::string prettyGoldValue(int64_t value) noexcept
{
    std::string result;
    int64_t silver = value / 100 % 100;
    int64_t copper = value % 100;
    int64_t gold = value / 10000;
    return fmt::format("{}G {}S {}C", gold, silver, copper);
}

void from_json(const nlohmann::json& j, TpInfo& tpInfo)
{
    tpInfo.id = j.value("id", static_cast<ItemId>(0));
    json buys = j.value("buys", json());
    tpInfo.buyValue = buys.value("unit_price", static_cast<int64_t>(0));
    json sells = j.value("sells", json());
    tpInfo.sellValue = sells.value("unit_price", static_cast<int64_t>(0));
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
