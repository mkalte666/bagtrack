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
    std::vector<int64_t> y2Vals = {};
    std::vector<std::string> tickStrings = {};
    std::vector<const char*> tickChars = {};
    ItemId itemId = 0;
    int64_t idStart = 0;
    int64_t idEnd = 0;

    int64_t max = 0;
    int64_t min = 0;
    float avg = 0;

    int64_t minDelta = 0;
    int64_t maxDelta = 0;
    int64_t last5Minutes = 0;

    int64_t minDelta15Minutes = 0;
    int64_t maxDelta15Minutes = 0;
    int64_t last15Minutes = 0;

    int64_t minDelta60Minutes = 0;
    int64_t maxDelta60Minutes = 0;
    int64_t last60Minutes = 0;
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