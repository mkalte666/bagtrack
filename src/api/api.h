// licence note at the end of the file

#ifndef BAGTRACK_API_H
#define BAGTRACK_API_H

#include "../settings.h"
#include "iteminfo.h"
#include "recipe.h"
#include "tpinfo.h"
#include <map>
#include <optional>
#include <set>

/**
 * \brief Item Counting structure that also keeps track of item binding
 */
struct ItemCount {
    int64_t bound = 0;
    int64_t unbound = 0;
    int64_t operator()() const noexcept { return bound + unbound; }
    ItemCount& operator+=(const ItemCount& other) noexcept
    {
        bound += other.bound;
        unbound += other.unbound;
        return *this;
    }
    ItemCount& operator-=(const ItemCount& other) noexcept
    {
        bound -= other.bound;
        unbound -= other.unbound;
        return *this;
    }
    ItemCount operator+(const ItemCount& other) const noexcept
    {
        ItemCount result = other;
        result += *this;
        return result;
    }
    ItemCount operator-(const ItemCount& other) const noexcept
    {
        ItemCount result = other;
        result -= *this;
        return result;
    }
    bool operator==(const ItemCount& other) const noexcept
    {
        return bound == other.bound && unbound == other.unbound;
    }
    bool operator!=(const ItemCount& other) const noexcept
    {
        return !(*this == other);
    }
};
/// json converter for ItemInfo
void from_json(const nlohmann::json& j, ItemCount& count);
/// json converter for ItemInfo
void to_json(nlohmann::json& j, const ItemCount& count);

/// map item ids to item counts
using ItemIdMap = std::map<ItemId, ItemCount>;
/// a collection of item IDs
using ItemIdList = std::vector<ItemId>;
/// map item ids to static item information
using ItemInfoMap = std::map<ItemId, ItemInfo>;

/**
 * \brief Validate the api key
 * \param key
 * \return
 */
bool checkApiKey(const std::string& key) noexcept;

/**
 * \brief Get the contents of the account bank
 * \param key
 * \return
 */
std::optional<ItemIdMap> getBankContents(const std::string& key) noexcept;

/**
 * \brief Get the contents of the accounts inventory storage
 * \param key
 * \return
 */
std::optional<ItemIdMap> getMaterialStorageContents(const std::string& key) noexcept;

/**
 * \brief Get the inventory contents of all game characters
 * \param key
 * \return
 */
std::optional<std::map<std::string, ItemIdMap>> getCharacterContents(const std::string& key) noexcept;

/**
 * \brief Get the content of the shared inventory slots
 * \param key
 * \return
 */
std::optional<ItemIdMap> getSharedInventory(const std::string& key) noexcept;

/**
 * \brief Get the number of coins in the account
 * \param key
 * \return
 */
int64_t getAccountCoins(const std::string& key) noexcept;

/**
 * \brief Get the static item information for the given set of ids
 * \param ids
 * \return
 */
ItemInfoMap getItemInfos(ItemIdList& ids) noexcept;

/**
 * \brief Get all the item ids that are in the api
 * \param ids
 * \return
 */
ItemIdList getAllItemIds() noexcept;

/**
 * \brief Get trading post infos for the given set of ids
 * \param ids
 * \return
 */
TpInfoMap getItemTpInfos(ItemIdList& ids) noexcept;

/**
 * \brief Get recipes for the given set of ids
 * \param recipes
 * \return
 */
std::vector<Recipe> getRecipes(RecipeIdList& recipes) noexcept;

/**
 * \brief Look up recipes used to craft item given with id
 * \param id
 * \return
 */
RecipeIdList getRecipesForItem(ItemId id) noexcept;

#endif //BAGTRACK_API_H
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