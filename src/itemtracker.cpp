// licence note at the end of the file

#include "itemtracker.h"
#include "fixfmt.h"

using namespace std::chrono_literals;

ItemTracker::ItemTracker(const Settings& settings) noexcept
{
    updateThread = std::thread([this, &settings]() { this->updateFunc(settings); });
}
ItemTracker::~ItemTracker() noexcept
{
    killer.kill();
    updateThread.join();
}

void ItemTracker::updateFunc(const Settings& settings) noexcept
{
    do {
        auto apiKey = settings.getApiKey();

        if (!checkApiKey(apiKey)) {
            fmt::print(stderr, "Api Key Invalid, tracker in pause");
            continue;
        }
        auto newItems = collectAllItemSources(apiKey);
        // error somewhere, or actually no items? move along
        if (newItems.empty()) {
            continue;
        }
        std::lock_guard lockGuard(mutex);
        std::swap(newItems, currentState);
        // also make sure we have some reference state
        isReferenceStateValid = true;
        if (referenceState.empty()) {
            referenceState = currentState;
        }
    } while (killer.wait_for(30s));
}

ItemIdMap ItemTracker::collectAllItemSources(const std::string& apiKey) noexcept
{
    auto mergeMap = [](ItemIdMap& dst, const ItemIdMap& src) {
        for (const auto& pair : src) {
            dst[pair.first] += pair.second;
        }
    };

    auto maybeMaterials = getMaterialStorageContents(apiKey);
    auto maybeBank = getBankContents(apiKey);
    auto maybeChars = getCharacterContents(apiKey);
    if (!maybeMaterials.has_value() || !maybeMaterials.has_value() || !maybeChars.has_value()) {
        return ItemIdMap();
    }

    ItemIdMap items = maybeMaterials.value();
    mergeMap(items, maybeBank.value());
    mergeMap(items, maybeChars.value());
    return items;
}

ItemIdMap ItemTracker::getCurrentState() const noexcept
{
    std::lock_guard lockGuard(mutex);
    ItemIdMap stateCopy = currentState;
    return stateCopy;
}

void ItemTracker::resetReferenceState() noexcept
{
    std::lock_guard lockGuard(mutex);

    if (!isReferenceStateValid) {
        return;
    }
    referenceState = currentState;
}

ItemIdMap ItemTracker::getFilteredDelta() const noexcept
{
    std::lock_guard lockGuard(mutex);
    if (!isReferenceStateValid) {
        return ItemIdMap();
    }

    ItemIdMap delta = currentState;
    for (const auto& pair : referenceState) {
        delta[pair.first] -= pair.second;
        if (delta[pair.first] == 0) {
            delta.erase(pair.first);
        }
    }

    return delta;
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
