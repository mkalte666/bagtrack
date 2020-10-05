// licence note at the end of the file
#include "trackerstate.h"

void from_json(const nlohmann::json& j, TrackerState& state)
{
    state.stateId = j.value("id", 0);
    state.items = j.value("items", ItemIdMap());
    state.coins = j.value("coins", 0);
}

void to_json(nlohmann::json& j, const TrackerState& state)
{
    j["id"] = state.stateId;
    j["items"] = state.items;
    j["coins"] = state.coins;
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
