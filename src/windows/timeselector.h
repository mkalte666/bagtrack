// licence note at the end of the file

#ifndef BAGTRACK_TIMESELECTOR_H
#define BAGTRACK_TIMESELECTOR_H

#include "../itemtracker.h"

/**
 * \brief
 * \param tracker
 * \param oldValue
 * \return
 */
int64_t timeSelector(const ItemTracker& tracker, int64_t oldValue = 0) noexcept;

#endif //BAGTRACK_TIMESELECTOR_H
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