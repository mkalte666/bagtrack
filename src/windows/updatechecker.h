// licence note at the end of the file

#ifndef BAGTRACK_UPDATECHECKER_H
#define BAGTRACK_UPDATECHECKER_H

#include "../version.h"
#include "window.h"
#include <future>

class UpdateChecker : public Window {
public:
    using Window::Window;
    UpdateChecker() noexcept;
    void update(Settings&, ItemTracker&, InfoCache&) noexcept override;

private:
    std::future<ReleaseInfo> releaseInfoResult = {};
    ReleaseInfo upstreamInfo = {};
};

#endif //BAGTRACK_UPDATECHECKER_H
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