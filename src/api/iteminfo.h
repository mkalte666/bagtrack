// licence note at the end of the file

#ifndef BAGTRACK_ITEMINFO_H
#define BAGTRACK_ITEMINFO_H

#include <nlohmann/json.hpp>
#include <string>

using ItemId = uint64_t;

/**
 * \brief Api item information
 */
struct ItemInfo {
    ItemId id = 0;
    std::string name = "Loading...";
    std::string description = {};
    std::string chatLink = {};
    std::string iconUrl = {};
    std::string type = {};
    std::string rarity = {};
    size_t level = {};
    int64_t vendorValue = {};
    std::vector<std::string> flags = {};

    [[nodiscard]] bool checkIfBound() const noexcept;
    [[nodiscard]] bool checkIfPrecursor() const noexcept;
};

/// json converter for ItemInfo
void from_json(const nlohmann::json& j, ItemInfo& itemInfo);
/// json converter for ItemInfo
void to_json(nlohmann::json& j, const ItemInfo& itemInfo);

#endif //BAGTRACK_ITEMINFO_H
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