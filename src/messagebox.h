// licence note at the end of the file

#ifndef REMUTE_MESSAGEBOX_H
#define REMUTE_MESSAGEBOX_H

#include <SDL.h>
#include <fmt/format.h>

/**
 * \brief Wrapper for SDL_ShowSimpleMessageBox with fmt formatting
 * \tparam Args
 * \param flags
 * \param window
 * \param title
 * \param fmt
 * \param args
 */
template <class... Args>
void messageBox(Uint32 flags, SDL_Window* window, const std::string& title, const std::string& fmt, Args... args)
{
    std::string message = fmt::format(fmt, std::forward<Args>(args)...);
    static_cast<void>(SDL_ShowSimpleMessageBox(flags, title.c_str(), message.c_str(), window));
}

/**
 * \brief Wrapper for SDL_ShowSimpleMessageBox with fmt formatting
 * \tparam Args
 * \param flags
 * \param title
 * \param fmt
 * \param args
 */
template <class... Args>
void messageBox(Uint32 flags, const std::string& title, const std::string& fmt, Args... args)
{
    messageBox(flags, nullptr, title, fmt, std::forward<Args>(args)...);
}

#endif //REMUTE_MESSAGEBOX_H

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