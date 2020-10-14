// licence note at the end of the file

#ifndef IMGUI_MISC_FMT_H
#define IMGUI_MISC_FMT_H

#include "fixfmt.h"
#include <imgui.h>

namespace ImGui {
template <class S, class... Args>
IMGUI_API void TextFmt(const S& fmt, Args&&... args)
{
    Text("%s", fmt::format(fmt, std::forward<Args>(args)...).c_str());
}

template <class S, class... Args>
IMGUI_API void TextColoredFmt(const ImVec4& col, const S& fmt, Args&&... args)
{
    TextColored(col, "%s", fmt::format(fmt, std::forward<Args>(args)...).c_str());
}

template <class S, class... Args>
IMGUI_API void TextDisabledFmt(const S& fmt, Args&&... args)
{
    TextDisabled("%s", fmt::format(fmt, std::forward<Args>(args)...).c_str());
}

template <class S, class... Args>
IMGUI_API void TextWrappedFmt(const S& fmt, Args&&... args)
{
    TextWrapped("%s", fmt::format(fmt, std::forward<Args>(args)...).c_str());
}

template <class SLabel, class S, class... Args>
IMGUI_API void LabelTextFmt(const SLabel& label, const S& fmt, Args&&... args)
{
    LabelText(label, "%s", fmt::format(fmt, std::forward<Args>(args)...).c_str());
}

template <class S, class... Args>
IMGUI_API void BulletTextFmt(const S& fmt, Args&&... args)
{
    BulletText("%s", fmt::format(fmt, std::forward<Args>(args)...).c_str());
}
}

#endif //IMGUI_MISC_FMT_H
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