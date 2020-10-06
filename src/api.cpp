// licence note at the end of the file

#include "api.h"

#include "fixfmt.h"
#include <httplib.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

httplib::Result makeRequest(const std::string& key, const std::string& endpoint, std::map<std::string, std::string> paramters = {})
{
    httplib::Client client("https://api.guildwars2.com");
    client.set_connection_timeout(10, 0);
    client.set_read_timeout(10, 0);
    client.set_write_timeout(10, 0);
    client.set_bearer_token_auth(key.c_str());
    std::string endpointWithParams = endpoint;
    // normal get paramters, endpoint?key1=value1&key2=value2 etc.
    char separator = '?'; // separator is ? at first and & for every other one
    for (const auto& pair : paramters) {
        endpointWithParams += separator + pair.first + '=' + pair.second;
        separator = '&'; // set the seperator to & for the rest of the parameters
    }
    printDebug("Getting {}\n", endpointWithParams);
    return client.Get(endpointWithParams.c_str());
}

bool checkResponseForValidToken(const httplib::Result& result)
{
    if (!result) {
        printDebug("result is broken!");
        return false;
    }
    if (result->status == 200) {
        printDebug("Token valid check success\n");
        return true;
    }

    if (result->status == 401) {
        printDebug("Token valid check failed - invalid token\n");
        return false;
    }

    auto keyInfo = json::parse(result->body);
    if (keyInfo.contains("text") && keyInfo["text"] == "Invalid access token") {
        printDebug("Token valid check failed - invalid token, but i also got a weird response id: {}\n", result->status);
        return false;
    }

    printDebug("Token valid check unknown, assuming valid for response id: {}\n", result->status);
    return true;
}

bool checkApiKey(const std::string& key) noexcept
{
    auto res = makeRequest(key, "/v2/tokeninfo");
    if (!checkResponseForValidToken(res)) {
        return false;
    }

    json keyInfo;
    try {
        keyInfo = json::parse(res->body);
    } catch (json::exception& e) {
        printDebug("Json parsing failed: {}", e.what());
        return false;
    }
    if (!keyInfo.contains("permissions")) {
        printDebug("Token check failed - invalid token information\n");
        return false;
    }

    try {
        std::vector<std::string> permissions = keyInfo["permissions"];
        if (std::find(permissions.begin(), permissions.end(), "account") == permissions.end()
            || std::find(permissions.begin(), permissions.end(), "inventories") == permissions.end()
            || std::find(permissions.begin(), permissions.end(), "characters") == permissions.end()
            || std::find(permissions.begin(), permissions.end(), "wallet") == permissions.end()) {
            printDebug("Token is missing permissions\n");
            return false;
        }
    } catch (const json::exception& e) {
        printDebug("Token info parsing failed, {}\n", e.what());
        return false;
    }

    printDebug("Token check successfull\n");
    return true;
}

std::optional<ItemIdMap> getBankContents(const std::string& key) noexcept
{
    auto result = makeRequest(key, "/v2/account/bank");
    if (!checkResponseForValidToken(result)) {
        printDebug("Cannot get bank contents!\n");
        return std::nullopt;
    }

    json j;
    try {
        j = json::parse(result->body);
    } catch (json::exception& e) {
        printDebug("Json parsing failed: {}", e.what());
        return std::nullopt;
    }
    if (!j.is_array()) {
        printDebug("Got unexpected contents from bank endpoint\n{}", j);
        return std::nullopt;
    }

    ItemIdMap items;

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

std::optional<ItemIdMap> getMaterialStorageContents(const std::string& key) noexcept
{

    auto result = makeRequest(key, "/v2/account/materials");
    if (!checkResponseForValidToken(result)) {
        printDebug("Cannot get material storage!\n");
        return std::nullopt;
    }

    json j;
    try {
        j = json::parse(result->body);
    } catch (json::exception& e) {
        printDebug("Json parsing failed: {}", e.what());
        return std::nullopt;
    }
    if (!j.is_array()) {
        printDebug("Got unexpected contents from materials endpoint\n{}", j);
        return std::nullopt;
    }

    ItemIdMap items;
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

std::optional<ItemIdMap> getCharacterContents(const std::string& key) noexcept
{
    auto charResult = makeRequest(key, "/v2/characters?ids=all");
    if (!checkResponseForValidToken(charResult)) {
        printDebug("Cannot get character list");
        return std::nullopt;
    }

    json charList;
    try {
        charList = json::parse(charResult->body);
    } catch (json::exception& e) {
        printDebug("Json parsing failed: {}", e.what());
        return std::nullopt;
    }

    if (!charList.is_array()) {
        printDebug("Unexpected character result tyoe");
        return std::nullopt;
    }

    ItemIdMap items;
    for (const auto& jsonChar : charList) {
        json equipmentList = jsonChar.value("equipment", json());
        if (equipmentList.is_array()) {
            for (const auto& item : equipmentList) {
                if (!item.is_object()) {
                    continue;
                }
                ItemId id = item.value("id", static_cast<ItemId>(0));
                ++items[id];
            }
        }
        json bagList = jsonChar.value("bags", json());
        if (bagList.is_array()) {
            for (const auto& bag : bagList) {
                if (!bag.is_object()) {
                    continue;
                }
                ItemId bagId = bag.value("id", static_cast<ItemId>(0));
                ++items[bagId];

                json bagInventory = bag.value("inventory", json());
                if (!bagInventory.is_array()) {
                    continue;
                }
                for (const auto& item : bagInventory) {
                    if (!item.is_object()) {
                        continue;
                    }
                    ItemId itemId = item.value("id", static_cast<ItemId>(0));
                    int64_t count = item.value("count", 0);
                    items[itemId] += count;
                }
            }
        }
    }

    return items;
}

int64_t getAccountCoins(const std::string& key) noexcept
{
    auto charResult = makeRequest(key, "/v2/account/wallet");
    if (!checkResponseForValidToken(charResult)) {
        printDebug("Cannot get character wallet");
        return 0;
    }

    json currencyList;
    try {
        currencyList = json::parse(charResult->body);
    } catch (json::exception& e) {
        printDebug("Json parsing failed: {}", e.what());
        return 0;
    }

    if (!currencyList.is_array()) {
        printDebug("Wallet endpoint gave unexpected result");
        return 0;
    }

    for (const auto& currency : currencyList) {
        if (!currency.is_object()) {
            continue;
        }

        if (currency.value("id", 0) == 1) { // 1 is gold coins
            return currency.value("value", static_cast<int64_t>(0));
        }
    }

    return 0;
}

ItemInfoMap getItemInfos(const std::set<ItemId>& ids) noexcept
{
    constexpr size_t maxIds = 150;
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
    json j;
    try {
        j = json::parse(res->body);
    } catch (json::exception& e) {
        printDebug("Json parsing failed: {}", e.what());
        return results;
    }
    if (!j.is_array()) {
        printDebug("Item endpoint returned wrong type - array expected, got shit\n");
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

TpInfoMap getItemTpInfos(const std::set<ItemId>& ids) noexcept
{
    constexpr size_t maxIds = 150;
    TpInfoMap results;
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
    auto res = makeRequest("", "/v2/commerce/prices", params);
    // small guard against failures
    if (!res || (res->status != 200 && res->status != 206)) {
        printDebug("TP endpoint connection failed\n");
        return results;
    }
    json j;
    try {
        j = json::parse(res->body);
    } catch (json::exception& e) {
        printDebug("Json parsing failed: {}", e.what());
        return results;
    }
    if (!j.is_array()) {
        printDebug("TP endpoint returned wrong type - array expected, got shit\n");
        return results;
    }

    for (const auto& itemJson : j) {
        if (!itemJson.is_object()) {
            continue;
        }
        TpInfo tpInfo;
        tpInfo.age = std::chrono::steady_clock::now();
        from_json(itemJson, tpInfo);
        results[tpInfo.id] = tpInfo;
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