// licence note at the end of the file

#include "recipe.h"

void from_json(const nlohmann::json& j, Recipe& recipe)
{
    recipe.id = j.value("id", 0ULL);
    recipe.product = j.value("product", 0ULL);
    recipe.guildProduct = j.value("guildProduct", 0ULL);
    recipe.productCount = j.value("productCount", 0LL);
    recipe.ingredients = j.value("ingredients", std::vector<Recipe::Ingredient>());
    recipe.guildIngredients = j.value("guildIngredients", std::vector<Recipe::Ingredient>());
    recipe.timeToCraft = j.value("timeToCraft", 0LL);
    recipe.disciplines = j.value("disciplines", std::vector<std::string>());
    recipe.minRating = j.value("minRating", 0LL);
    recipe.flags = j.value("flags", std::vector<std::string>());
    recipe.chatLink = j.value("chatLink", "");
}

void to_json(nlohmann::json& j, const Recipe& recipe)
{
    j["id"] = recipe.id;
    j["product"] = recipe.product;
    j["guildProduct"] = recipe.guildProduct;
    j["productCount"] = recipe.productCount;
    j["ingredients"] = recipe.ingredients;
    j["guildIngredients"] = recipe.guildIngredients;
    j["timeToCraft"] = recipe.timeToCraft;
    j["disciplines"] = recipe.disciplines;
    j["minRating"] = recipe.minRating;
    j["flags"] = recipe.flags;
    j["chatLink"] = recipe.flags;
}

void from_json(const nlohmann::json& j, Recipe::Ingredient& ingredient)
{
    ingredient.id = j.value("id", 0ULL);
    ingredient.count = j.value("count", 0LL);
}

void to_json(nlohmann::json& j, const Recipe::Ingredient& ingredient)
{
    j["id"] = ingredient.id;
    j["count"] = ingredient.count;
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