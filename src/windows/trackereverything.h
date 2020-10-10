// licence note at the end of the file

#ifndef BAGTRACK_TRACKEREVERYTHING_H
#define BAGTRACK_TRACKEREVERYTHING_H

#include "itemwidget.h"
#include "statsgraph.h"
#include "window.h"
class TrackerEverything : public Window {
public:
    using Window::Window;
    TrackerEverything() noexcept;

    void update(Settings&, ItemTracker&, InfoCache&) noexcept override;
    void drawMainMenu() noexcept override;

private:
    ItemWidgetState widgetState;
    int64_t referenceId = 0;
    StatsGraph statsGraph = StatsGraph("EverythingTracker");
};

#endif //BAGTRACK_TRACKEREVERYTHING_H
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