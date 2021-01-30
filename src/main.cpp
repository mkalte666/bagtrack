// licence note at the end of the file

#include "messagebox.h"
#include <SDL.h>

// clang-format off
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl.h>
#include <GL/gl3w.h>
#include <implot.h>
// clang-format on

#include <vector>
#include <fstream>

#include "itemtracker.h"
#include "api/infocache.h"

#include "windows/apikeyeditor.h"
#include "windows/itemselector.h"
#include "windows/trackereverything.h"
#include "windows/goldtracker.h"
#include "windows/filedialog.h"
#include "windows/updatechecker.h"
#include "windows/cacheallbutton.h"
#include "windows/statusbar.h"
#include "windows/itemlookup.h"
#include "../assets/bootleg_baggie.bmp.cpp"

void debugToFile(const std::string& str)
{
    static std::ofstream out(Settings::getPrefPath() / "log.txt");
    out << str;
    out.flush();
}

constexpr float footerSize = 30.0F;

int main(int, char**)
{
    int sdlRes = SDL_Init(SDL_INIT_VIDEO);
    if (sdlRes != 0) {
        messageBox(SDL_MESSAGEBOX_ERROR, "SDL ERROR", "Could not Init SDL: {}", SDL_GetError());
        return 1;
    }
    // write something to the logfile super early (after sdl init) to get the outfile stream to be there
    debugToFile("logfile opened\n");
    std::set_terminate([]() {
        debugToFile("\n!!!Unhandled Exception! dying now\n");
        std::abort();
    });
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    const char* glsl_version = "#version 130";

    //windowpos is doing shitstuff NOLINTNEXTLINE
    auto* window = SDL_CreateWindow("Bag Track", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        messageBox(SDL_MESSAGEBOX_ERROR, "SDL ERROR", "Could not create window: {}", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // load icon. this is annoying
    SDL_RWops* icon = SDL_RWFromMem(assets_bootleg_baggie_bmp, static_cast<int>(assets_bootleg_baggie_bmp_len));
    if (icon != nullptr) {
        SDL_Surface* iconSurface = SDL_LoadBMP_RW(icon, 1); // free the rw
        if (iconSurface != nullptr) {
            SDL_SetWindowIcon(window, iconSurface);
            SDL_FreeSurface(iconSurface);
        }
    }

    auto* context = SDL_GL_CreateContext(window);
    if (context == nullptr) {
        messageBox(SDL_MESSAGEBOX_ERROR, "SDL ERROR", "Could not create OpenGL Context: {}", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_GL_MakeCurrent(window, context);
    SDL_GL_SetSwapInterval(1);

    // glew has a slightly different signature, but they are compatible NOLINTNEXTLINE
    auto gl3wres = gl3wInit2(reinterpret_cast<GL3WGetProcAddressProc>(&SDL_GL_GetProcAddress));
    if (gl3wres != GL3W_OK) {
        messageBox(SDL_MESSAGEBOX_WARNING, "GL Error", "OpenGL version not supported. App might just crash");
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImPlot::CreateContext();
    ImPlot::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO();
    std::string iniFileName = (Settings::getPrefPath() / "imgui.ini").string();
    io.IniFilename = iniFileName.c_str();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // NOLINT
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // NOLINT
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // NOLINT
    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    bool running = true;
    Uint64 timeAtStart = SDL_GetPerformanceCounter();
    auto ticksPerSecond = static_cast<double>(SDL_GetPerformanceFrequency());

    Settings settings;
    ItemTracker tracker(settings);
    InfoCache infoCache;
    StatusBar statusBar;
    std::vector<std::unique_ptr<Window>> windows;
    windows.emplace_back(std::make_unique<FileDialog>());
    windows.emplace_back(std::make_unique<ApiKeyEditor>());
    windows.emplace_back(std::make_unique<ItemSelector>());
    windows.emplace_back(std::make_unique<TrackerEverything>());
    windows.emplace_back(std::make_unique<GoldTracker>());
    windows.emplace_back(std::make_unique<UpdateChecker>());
    windows.emplace_back(std::make_unique<CacheAllButton>());
    windows.emplace_back(std::make_unique<ItemLookup>());

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (ImGui_ImplSDL2_ProcessEvent(&e)) {
                continue;
            }
            if (e.type == SDL_QUIT) {
                running = false;
            }
        }
        int winW = 0;
        int winH = 0;
        SDL_GetWindowSize(window, &winW, &winH);
        glClearColor(0.0F, 0.0F, 0.0F, 1.0F); // NOLINT
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // NOLINT

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, viewport->Size.y - footerSize));
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::SetNextWindowBgAlpha(0.0F);

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove; // NOLINT
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus; // NOLINT

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0F);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0F);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0F, 0.0F));
        ImGui::Begin("Dock Windows Here", nullptr, window_flags);
        ImGui::PopStyleVar(3);

        ImGuiID dockspace_id = ImGui::GetID("Dock windows here");
        ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_AutoHideTabBar;
        ImGui::DockSpace(dockspace_id, ImVec2(0.0F, 0.0F), dockspace_flags);
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + viewport->Size.y - footerSize));
        ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, footerSize));
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImColor(20, 20, 20, 255).Value);
        ImGui::Begin("FOOTER", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings);
        statusBar.drawStatusBar(infoCache);
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
        ImGui::End();

        auto time = static_cast<double>(SDL_GetPerformanceCounter() - timeAtStart) / ticksPerSecond;
        static_cast<void>(time);
        // code goes here

        // draw menus in all of the windows
        ImGui::BeginMainMenuBar();
        for (auto& w : windows) {
            w->drawMainMenu();
        }
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit")) {
                running = false;
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();

        // draw all of the windows
        for (auto& w : windows) {
            w->update(settings, tracker, infoCache);
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(window, context);

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
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