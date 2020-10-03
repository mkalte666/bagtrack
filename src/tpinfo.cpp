// Licence note is at the end of the file

#include "tpinfo.h"
#include "fixfmt.h"

using json = nlohmann::json;

std::string prettyGoldValue(uint64_t value) noexcept
{
    std::string result;
    uint64_t silver = value / 100U % 100U;
    uint64_t copper = value % 100U;
    uint64_t gold = value / 10000;
    return fmt::format("{}G {}S {}C", gold, silver, copper);
}

void from_json(const nlohmann::json& j, TpInfo& tpInfo)
{
    tpInfo.id = j.value("id", static_cast<ItemId>(0));
    json buys = j.value("buys", json());
    tpInfo.buyValue = buys.value("unit_price", static_cast<uint64_t>(0));
    json sells = j.value("sells", json());
    tpInfo.sellValue = sells.value("unit_price", static_cast<uint64_t>(0));
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
