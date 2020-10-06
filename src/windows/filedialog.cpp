// licence note at the end of the file

#include "filedialog.h"
#include <nfd.h>

#include <imgui.h>

void FileDialog::update(Settings& settings, ItemTracker& tracker, InfoCache&) noexcept
{
    switch (nextAction) {

    case NextAction::None:
        break;
    case NextAction::New: {
        nfdchar_t* outPath = nullptr;
        nfdresult_t result = NFD_SaveDialog("bagtrack", settings.getLastHistoryFile().remove_filename().string().c_str(), &outPath);
        if (result == NFD_OKAY) {
            fs::path p = outPath;
            free(outPath); // NOLINT api does that
            if (p.extension() != ".bagtrack") {
                p += ".bagtrack";
            }
            tracker.writeCache(settings.getLastHistoryFile());
            tracker.clearHistory();
            settings.setLastHistoryFile(p);
            tracker.writeCache(p);
        }
        break;
    }
    case NextAction::Open: {
        nfdchar_t* outPath = nullptr;
        nfdresult_t result = NFD_OpenDialog("bagtrack", settings.getLastHistoryFile().remove_filename().string().c_str(), &outPath);
        if (result == NFD_OKAY) {
            fs::path p = outPath;
            free(outPath); // NOLINT api does that
            tracker.writeCache(settings.getLastHistoryFile());
            tracker.clearHistory();
            tracker.readCache(p);
        }
        break;
    }
    case NextAction::SaveAs: {
        nfdchar_t* outPath = nullptr;
        nfdresult_t result = NFD_SaveDialog("bagtrack", settings.getLastHistoryFile().remove_filename().string().c_str(), &outPath);
        if (result == NFD_OKAY) {
            fs::path p = outPath;
            free(outPath); // NOLINT api does that
            if (p.extension() != ".bagtrack") {
                p += ".bagtrack";
            }
            tracker.writeCache(settings.getLastHistoryFile());
            settings.setLastHistoryFile(p);
            tracker.writeCache(p);
        }
        break;
    }
    }

    nextAction = NextAction::None;
}

void FileDialog::drawMainMenu() noexcept
{
    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("New Session")) {
            nextAction = NextAction::New;
        }
        if (ImGui::MenuItem("Open Session")) {
            nextAction = NextAction::Open;
        }
        if (ImGui::MenuItem("Save Session As...")) {
            nextAction = NextAction::SaveAs;
        }
        ImGui::Separator();
        ImGui::EndMenu();
    }
}

FileDialog::FileDialog() noexcept
    : Window("file dialog")
{
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
