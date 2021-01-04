// licence note at the end of the file
#ifndef REMUTE_WINDOW_H
#define REMUTE_WINDOW_H

#include "../api/infocache.h"
#include "../itemtracker.h"
#include "../settings.h"
#include <string>

/**
 * \brief Mini wrapper for statefull gui windows. Kinda agains the spirit of imgui but i dont care
 */
class Window {
public:
    constexpr static float DefaultSquareWindowSize = 400.0F;
    Window() noexcept = default;
    explicit Window(std::string name) noexcept;
    Window(const Window&) = delete;
    Window(Window&&) = default;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = default;
    virtual ~Window() noexcept = default;
    virtual void update(Settings&, ItemTracker&, InfoCache&) noexcept;
    virtual void drawMainMenu() noexcept;

    void setShown(bool shown);

protected:
    void checkShownStateAgainstSettings(Settings& settings) noexcept;
    bool shown = false; // NOLINT makes interating with imgui Begin() a lot easier
    std::string name = ""; // NOLINT makes interacting with imgui Begin() a lot easier

private:
    bool shownWasCheckedAlready = false;
};

#endif //REMUTE_WINDOW_H
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