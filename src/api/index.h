// licence note at the end of the file

#ifndef BAGTRACK_INDEX_H
#define BAGTRACK_INDEX_H

#include "iteminfo.h"
#include <string>

class Index {
public:
    Index() noexcept;
    ~Index() noexcept;
    Index(const Index&) = delete;
    Index(Index&&) = delete;
    Index& operator=(const Index&) = delete;
    Index& operator=(Index&&) = delete;

    void addItem(const ItemInfo& info) noexcept;
    [[nodiscard]] std::vector<ItemId> find(const std::string& s) const;

    [[nodiscard]] bool hasAnyData() const noexcept;

private:
    void indexString(const std::string_view& s, ItemId id) noexcept;
    void indexWord(const std::string_view& s, ItemId id) noexcept;
    void insert(size_t hash, ItemId id);
    /// read cache from the disk
    void readCache();
    /// write cache to disk
    void writeCache() const;
    using StringIndex = std::map<size_t, std::vector<ItemId>>;
    StringIndex index = {};
};

#endif //BAGTRACK_INDEX_H
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