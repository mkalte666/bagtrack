// licence note at the end of the file
#ifndef BAGTRACK_TRACKERSTATE_H
#define BAGTRACK_TRACKERSTATE_H

#include "api/api.h"
#include "api/iteminfo.h"

/**
 * \brief A snapshot for the item Tracker
 */
struct TrackerState {
    /**
     * \brief Point-in-time based state id.
     * every 5 minutes since unix epoch = increase by one
     */
    int64_t stateId = {};
    /// Items in the inventory
    ItemIdMap items = {};
    /// Account gold coins
    int64_t coins = {};
};

/**
 * \brief Json converter for TrackerState
 * \param j
 * \param state
 */
void from_json(const nlohmann::json& j, TrackerState& state);

/**
 * \brief Json converter for TrackerState
 * \param j
 * \param state
 */
void to_json(nlohmann::json& j, const TrackerState& state);

#endif //BAGTRACK_TRACKERSTATE_H
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