// licence note at the end of the file

#include "recipe.h"

void from_json(const nlohmann::json& j, Recipe& recipe)
{
    recipe.id = j.value("id", 0ULL);
    recipe.type = j.value("type", "");
    recipe.product = j.value("output_item_id", 0ULL);
    recipe.guildProduct = j.value("output_upgrade_id", 0ULL);
    recipe.productCount = j.value("output_item_count", 0LL);
    recipe.ingredients = j.value("ingredients", std::vector<Recipe::Ingredient>());
    recipe.guildIngredients = j.value("guild_ingredients", std::vector<Recipe::Ingredient>());
    recipe.timeToCraft = j.value("time_to_craft_ms", 0LL);
    recipe.disciplines = j.value("disciplines", std::vector<std::string>());
    recipe.minRating = j.value("min_rating", 0LL);
    recipe.flags = j.value("flags", std::vector<std::string>());
    recipe.chatLink = j.value("chat_link", "");
}

void to_json(nlohmann::json& j, const Recipe& recipe)
{
    j["id"] = recipe.id;
    j["type"] = recipe.type;
    j["output_item_id"] = recipe.product;
    j["output_upgrade_id"] = recipe.guildProduct;
    j["output_item_count"] = recipe.productCount;
    j["ingredients"] = recipe.ingredients;
    j["guild_ingredients"] = recipe.guildIngredients;
    j["time_to_craft_ms"] = recipe.timeToCraft;
    j["disciplines"] = recipe.disciplines;
    j["min_rating"] = recipe.minRating;
    j["flags"] = recipe.flags;
    j["chat_link"] = recipe.flags;
}

void from_json(const nlohmann::json& j, Recipe::Ingredient& ingredient)
{
    ingredient.id = j.value("item_id", 0ULL);
    ingredient.count = j.value("count", 0LL);
}

void to_json(nlohmann::json& j, const Recipe::Ingredient& ingredient)
{
    j["item_id"] = ingredient.id;
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