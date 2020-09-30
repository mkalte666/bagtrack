// licence note at the end of the file

#include "api.h"

#include <fmt/format.h>
#include <httplib.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

httplib::Result makeRequest(const std::string& key, const std::string& endpoint, std::map<std::string, std::string> paramters = {})
{
    httplib::Client client("https://api.guildwars2.com");
    client.set_bearer_token_auth(key.c_str());
    std::string endpointWithParams = endpoint;
    // normal get paramters, endpoint?key1=value1&key2=value2 etc.
    char separator = '?'; // separator is ? at first and & for every other one
    for (const auto& pair : paramters) {
        endpointWithParams += separator + pair.first + '=' + pair.second;
        separator = '&'; // set the seperator to & for the rest of the parameters
    }
    fmt::print(stderr, "Getting {}\n", endpointWithParams);
    return client.Get(endpointWithParams.c_str());
}

bool checkResponseForValidToken(const httplib::Result& result)
{
    if (!result) {
        fmt::print(stderr, "result is broken!");
    }
    if (result->status == 200) {
        fmt::print(stderr, "Token valid check success\n");
        return true;
    }

    if (result->status == 401) {
        fmt::print(stderr, "Token valid check failed - invalid token\n");
        return false;
    }

    auto keyInfo = json::parse(result->body);
    if (keyInfo.contains("text") && keyInfo["text"] == "Invalid access token") {
        fmt::print(stderr, "Token valid check failed - invalid token, but i also got a weird response id: {}\n", result->status);
        return false;
    }

    fmt::print(stderr, "Token valid check unknown, assuming valid for response id: {}\n", result->status);
    return true;
}

bool checkApiKey(const std::string& key) noexcept
{
    auto res = makeRequest(key, "/v2/tokeninfo");
    if (!checkResponseForValidToken(res)) {
        return false;
    }

    auto keyInfo = json::parse(res->body);
    if (!keyInfo.contains("permissions")) {
        fmt::print(stderr, "Token check failed - invalid token information\n");
        return false;
    }

    try {
        std::vector<std::string> permissions = keyInfo["permissions"];
        if (std::find(permissions.begin(), permissions.end(), "account") == permissions.end()
            || std::find(permissions.begin(), permissions.end(), "inventories") == permissions.end()
            || std::find(permissions.begin(), permissions.end(), "characters") == permissions.end()
            || std::find(permissions.begin(), permissions.end(), "wallet") == permissions.end()) {
            fmt::print(stderr, "Token is missing permissions\n");
            return false;
        }
    } catch (const json::exception& e) {
        fmt::print(stderr, "Token info parsing failed, {}\n", e.what());
        return false;
    }

    fmt::print(stderr, "Token check successfull\n");
    return true;
}

ItemIdMap getBankContents(const std::string& key) noexcept
{
    ItemIdMap items;
    auto result = makeRequest(key, "/v2/account/bank");
    if (!checkResponseForValidToken(result)) {
        fmt::print(stderr, "Cannot get bank contents!\n");
        return items;
    }

    auto j = json::parse(result->body);
    if (!j.is_array()) {
        fmt::print(stderr, "Got unexpected contents from bank endpoint\n{}", j);
    }
    for (const auto& item : j) {
        if (item.is_null()) { // no worries, null just means the slot es empty
            continue;
        }
        ItemId id = item.value("id", static_cast<ItemId>(0));
        int64_t count = item.value("count", 0);
        items[id] += count;
    }
    return items;
}

ItemIdMap getMaterialStorageContents(const std::string& key) noexcept
{
    ItemIdMap items;
    auto result = makeRequest(key, "/v2/account/materials");
    if (!checkResponseForValidToken(result)) {
        fmt::print(stderr, "Cannot get material storage!\n");
        return items;
    }

    auto j = json::parse(result->body);
    if (!j.is_array()) {
        fmt::print(stderr, "Got unexpected contents from materials endpoint\n{}", j);
    }
    for (const auto& item : j) {
        if (item.is_null()) { // no worries, null just means the slot es empty
            continue;
        }
        ItemId id = item.value("id", static_cast<ItemId>(0));
        int64_t count = item.value("count", 0);
        items[id] += count;
    }
    return items;
}

ItemInfoMap getItemInfos(const std::set<ItemId>& ids) noexcept
{
    constexpr size_t maxIds = 50;
    ItemInfoMap results;
    std::string idString;
    size_t count = 0;
    for (const auto& id : ids) {
        idString += std::to_string(id) + ",";
        ++count;
        if (count > maxIds) {
            break;
        }
    }

    std::map<std::string, std::string> params;
    params["ids"] = idString;
    auto res = makeRequest("", "/v2/items", params);
    // small guard against failures
    if (!res || (res->status != 200 && res->status != 206)) {
        return results;
    }
    auto j = json::parse(res->body);
    if (!j.is_array()) {
        fmt::print(stderr, "Item endpoint returned wrong type - array expected, got shit\n");
        return results;
    }

    for (const auto& itemJson : j) {
        if (!itemJson.is_object()) {
            continue;
        }
        ItemInfo item;
        from_json(itemJson, item);
        results[item.id] = item;
    }
    return results;
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