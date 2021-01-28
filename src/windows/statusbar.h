// licence note at the end of the file

#ifndef BAGTRACK_STATUSBAR_H
#define BAGTRACK_STATUSBAR_H

#include "api/infocache.h"

class StatusBar {
public:
    void drawStatusBar(const InfoCache& infoCache) noexcept;

private:
    size_t itemCount = 0;
    size_t tpCount = 0;
};

#endif //BAGTRACK_STATUSBAR_H
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