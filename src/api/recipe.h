// licence note at the end of the file

#ifndef BAGTRACK_RECIPE_H
#define BAGTRACK_RECIPE_H

#include "iteminfo.h"

struct Recipe {
    struct Ingredient {
        ItemId id = 0;
        int64_t count = 0;
    };
    ItemId product = 0;
    ItemId guildProduct = 0;
    int64_t productCount = 0;
    std::vector<Ingredient> ingredients = {};
    std::vector<Ingredient> guildIngredients = {};
    std::string type = {};
    int64_t timeToCraft = {};
    std::vector<std::string> disciplines = {};
    int64_t minRating = 0;
    std::vector<std::string> flags = {};
    std::string chatLink = {};
};

/// json converter for Recipe
void from_json(const nlohmann::json& j, Recipe& recipe);
/// json converter for Recipe
void to_json(nlohmann::json& j, const Recipe& recipe);
/// json converter for Recipe::Ingredient
void from_json(const nlohmann::json& j, Recipe::Ingredient& ingredient);
/// json converter for Recipe::Ingredient
void to_json(nlohmann::json& j, const Recipe::Ingredient& ingredient);

#endif //BAGTRACK_RECIPE_H
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