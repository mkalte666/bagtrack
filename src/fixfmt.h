// licence note at the end of the file

#ifndef BAGTRACK_FIXFMT_H
#define BAGTRACK_FIXFMT_H

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Weffc++"
#endif

#include <fmt/format.h>

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

void debugToFile(const std::string& str);

#ifndef NDEBUG
template <class StringT, class... Args>
void printDebug(const StringT& fmt, Args... args)
{
    const std::string str = fmt::format(fmt, std::forward<Args>(args)...);
    fmt::print(stderr, str);
    debugToFile(str);
}
#else
template <class StringT, class... Args>
void printDebug(const StringT& fmt, Args... args)
{
    const std::string str = fmt::format(fmt, std::forward<Args>(args)...);
    debugToFile(str);
}
#endif

#endif //BAGTRACK_FIXFMT_H
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
