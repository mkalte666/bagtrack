// licence note at the end of the file

#ifndef BAGTRACK_STATSGRAPH_H
#define BAGTRACK_STATSGRAPH_H

#include "window.h"

class StatsGraph : public Window {
public:
    using Window::Window;

    void update(Settings&, ItemTracker&, InfoCache&) noexcept override;

    void setItem(ItemId id) noexcept;
    void setTimeslot(int64_t start, int64_t end) noexcept;

private:
    void rebuildStats() noexcept;
    bool needRebuild = false;
    std::map<int64_t, int64_t> statCache = {};
    std::vector<int64_t> xVals = {};
    std::vector<int64_t> yVals = {};
    std::vector<std::string> tickStrings = {};
    std::vector<const char*> tickChars = {};
    ItemId itemId = 0;
    int64_t idStart = 0;
    int64_t idEnd = 0;

    int64_t max = 0.0F;
    int64_t min = 0.0F;
    float avg = 0.0F;
};

#endif //BAGTRACK_STATSGRAPH_H
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