// licence note at the end of the file

#include "index.h"
#include "settings.h"
#include "string_view"
#include <fstream>

// toLower modies chars but this is not too nice on the compiler warnings
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#endif
std::string reduceString(const std::string_view in)
{
    std::string result;
    result.reserve(in.size());
    for (char c : in) {
        if (c >= 'A' && c <= 'Z') {
            c += 32;
        }
        if (c == '\n' || c == '\t' || c == '\r' || c == ' ' || c == '\'' || c == '\"') {
            continue;
        }

        result.push_back(c);
    }
    return result;
}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

Index::Index() noexcept
{
    readCache();
}

Index::~Index() noexcept
{
    writeCache();
}

void Index::addItem(const ItemInfo& info) noexcept
{
    indexString(info.name, info.id);
    indexString(info.description, info.id);
    indexString(info.chatLink, info.id);
    indexString(info.rarity, info.id);
}

std::vector<ItemId> Index::find(const std::string_view s) const
{
    const std::string reduced = reduceString(s);
    if (const auto iter = index.find(std::hash<std::string_view> {}(reduced)); iter != index.end()) {
        return iter->second;
    }
    return std::vector<ItemId>();
}

void Index::indexString(const std::string_view in, const ItemId id) noexcept
{
    // reduced whole thing first
    const std::string s = reduceString(in);
    indexWord(s, id);

    // we reduce this thing into words.
    // then push current word, and the two strings from and including the current word
    constexpr const char* space = "\n\t\r ";
    size_t pos = in.find_first_of(space);
    size_t startPos = 0;
    while (pos != std::string_view::npos) {
        const auto word = in.substr(startPos, pos - startPos);
        indexWord(word, id);
        const auto left = in.substr(0, pos);
        indexWord(left, id);
        const auto right = in.substr(startPos, in.size());
        indexWord(right, id);
        startPos = pos + 1;
        pos = in.find_first_of(space, startPos);
    }
    indexWord(in.substr(startPos, in.size()), id);
}

void Index::indexWord(const std::string_view in, const ItemId id) noexcept
{
    std::string s = reduceString(in);
    if (s.size() < 3) {
        return;
    }

    size_t hash = std::hash<std::string> {}(s);
    insert(hash, id);
}

void Index::insert(const size_t hash, const ItemId id)
{
    auto iter = index.find(hash);
    // if the hash does not exist, just throw it in
    if (iter == index.end()) {
        index[hash] = { id };
    } else {
        // otherwise make sure we dont duplicate
        auto& vec = iter->second;
        if (auto vecIter = std::find(vec.begin(), vec.end(), id); vecIter == vec.end()) {
            vec.push_back(id);
        }
    }
}

void Index::readCache()
{
    auto cacheFileName = Settings::getPrefPath();
    cacheFileName /= "searchIndex.json";
    std::ifstream cacheFile(cacheFileName);
    if (cacheFile.is_open()) {
        nlohmann::json j;
        cacheFile >> j;
        index = j.value("index", StringIndex());
    }
}

void Index::writeCache() const
{
    auto cacheFileName = Settings::getPrefPath();
    cacheFileName /= "searchIndex.json";
    std::ofstream cacheFile(cacheFileName);
    if (cacheFile.good()) {
        nlohmann::json j;
        j["index"] = index;
        cacheFile << j;
    }
}

bool Index::hasAnyData() const noexcept
{
    return !index.empty();
}

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
