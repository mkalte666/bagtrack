// licence note at the end of the file
#include "iteminfo.h"

void from_json(const nlohmann::json& j, ItemInfo& itemInfo)
{
    itemInfo.id = j.value("id", 0);
    itemInfo.name = j.value("name", "");
    itemInfo.description = j.value("description", "");
    itemInfo.chatLink = j.value("chat_link", "");
    itemInfo.iconUrl = j.value("icon", "");
    itemInfo.type = j.value("type", "");
    itemInfo.rarity = j.value("rarity", "");
    itemInfo.level = j.value("level", 0);
    itemInfo.vendorValue = j.value("vendor_value", 0);
}

void to_json(nlohmann::json& j, const ItemInfo& itemInfo)
{
    j["id"] = itemInfo.id;
    j["name"] = itemInfo.name;
    j["description"] = itemInfo.description;
    j["chat_link"] = itemInfo.chatLink;
    j["icon"] = itemInfo.iconUrl;
    j["type"] = itemInfo.type;
    j["rarity"] = itemInfo.rarity;
    j["level"] = itemInfo.level;
    j["vendor_value"] = itemInfo.vendorValue;
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