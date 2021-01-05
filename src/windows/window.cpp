// licence note at the end of the file

#include "window.h"
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

#include <imgui.h>

#include <utility>

Window::Window(std::string newname) noexcept
    : name(std::move(newname))
{
}

void Window::update(Settings&, ItemTracker&, InfoCache&) noexcept
{
}

void Window::drawMainMenu() noexcept
{
}

void Window::checkShownStateAgainstSettings(Settings& settings) noexcept
{
    if (!shownWasCheckedAlready) {
        shownWasCheckedAlready = true;
        shown = settings.getWindowShown(name);
        return;
    }

    bool savedShown = settings.getWindowShown(name);
    if (savedShown != shown) {
        settings.setWindowShown(name, shown);
    }
}

void Window::setShown(bool b)
{
    shown = b;
}
