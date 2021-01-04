// licence note at the end of the file
#ifndef BAGTRACK_VERSION_H
#define BAGTRACK_VERSION_H

#include <string>

static constexpr int BagtrackVersionMajor = 0;
static constexpr int BagtrackVersionMinor = 10;
static constexpr int BagtrackVersionPatch = 2;

struct VersionInfo {
    int major = 0;
    int minor = 0;
    int patch = 0;

    bool operator<(const VersionInfo& other) const noexcept
    {
        return major < other.major || (major == other.major && minor < other.minor) || (major == other.major && minor == other.minor && patch < other.patch);
    }
};

struct ReleaseInfo {
    VersionInfo version = {};
    std::string title = {};
    std::string text = {};
};

static constexpr VersionInfo BagtrackVersion = {
    BagtrackVersionMajor,
    BagtrackVersionMinor,
    BagtrackVersionPatch
};

#endif //BAGTRACK_VERSION_H
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