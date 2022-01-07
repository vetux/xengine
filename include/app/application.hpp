/**
 *  Mana - 3D Game Engine
 *  Copyright (C) 2021  Julian Zampiccoli
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef MANA_APPLICATION_HPP
#define MANA_APPLICATION_HPP

#include <chrono>
#include <thread>
#include <iomanip>

#include "ecs/ecs.hpp"
#include "io/archive.hpp"
#include "compat/imguicompat.hpp"

#include "platform/display/displaymanager.hpp"

#include "imgui.h"
#include "implot.h"

namespace engine {
    class MANA_EXPORT Application {
    public:
        /**
         * @param argc
         * @param argv
         * @param archive The archive instance to use as root archive.
         */
        explicit Application(int argc, char *argv[], std::unique_ptr<Archive> archive)
                : archive(std::move(archive)), display() {
            std::vector<std::string> args;
            for (int i = 0; i < argc; i++)
                args.emplace_back(argv[i]);

            displayBackend = DisplayBackend::GLFW;
            for (int i = 0; i < args.size(); i++) {
                if (args.at(i) == "--display") {
                    auto str = args.at(i + 1);
                    if (str == "glfw") {
                        displayBackend = DisplayBackend::GLFW;
                    }
                }
            }
            display = DisplayManager(displayBackend);

            graphicsBackend = GraphicsBackend::OPENGL_4_6;
            for (int i = 0; i < args.size(); i++) {
                if (args.at(i) == "--graphics") {
                    auto str = args.at(i + 1);
                    if (str == "opengl") {
                        graphicsBackend = GraphicsBackend::OPENGL_4_6;
                    } else if (str == "directx") {
                        graphicsBackend = GraphicsBackend::DIRECTX_11;
                    } else if (str == "vulkan") {
                        graphicsBackend = GraphicsBackend::VULKAN;
                    }
                }
            }
            window = display.createWindow();
            window->update();

            renderDevice = RenderDevice::create(graphicsBackend);

            imGuiContext = ImGui::CreateContext();
            ImGuiCompat::Init(*window, graphicsBackend);

            imPlotContext = ImPlot::CreateContext();
        }

        virtual ~Application() {
            ImPlot::DestroyContext(imPlotContext);

            ImGuiCompat::Shutdown(*window, graphicsBackend);
            ImGui::DestroyContext(imGuiContext);
        }

        virtual int loop() {
//Disable catch all exception handler in debug build to be able to use debugger exception handler
#ifndef MANA_DEBUG_BUILD
            try {
#endif
            start();
            auto lastFrame = std::chrono::high_resolution_clock::now();
            float deltaTime = 0;
            while (!window->shouldClose() && !shutdown) {
                auto frameStart = std::chrono::high_resolution_clock::now();
                update(deltaTime);
                auto frameDelta = std::chrono::high_resolution_clock::now() - frameStart;
                deltaTime = static_cast<float>(frameDelta.count()) / 1000000000.0f;
            }
            stop();
#ifndef MANA_DEBUG_BUILD
            }
            catch (const std::exception &e) {
                // Show uncaught exception dialog
                auto time = std::chrono::system_clock::now();
                auto time_t = std::chrono::system_clock::to_time_t(time);
                std::stringstream stream;
                stream << std::put_time(std::localtime(&time_t), "%Y-%m-%d %X");
                auto timeStr = stream.str();

                while (true) {
                    window->update();

                    ImGuiCompat::NewFrame(*window, graphicsBackend);
                    ImGui::NewFrame();

                    ImGui::Begin(("Uncaught Exception " + timeStr).c_str());

                    ImGui::Text("%s", e.what());

                    ImGui::Separator();

                    if (ImGui::Button("Quit") || window->shouldClose()) {
                        break;
                    }

                    ImGui::Dummy({600, 0});

                    ImGui::End();

                    ImGui::Render();
                    ImGuiCompat::DrawData(*window, window->getRenderTarget(graphicsBackend), graphicsBackend);

                    window->swapBuffers();
                }
            }
#endif
            return 0;
        }

    protected:
        DisplayManager display;

        DisplayBackend displayBackend;
        GraphicsBackend graphicsBackend;

        std::unique_ptr<Window> window = nullptr;
        std::unique_ptr<RenderDevice> renderDevice = nullptr;

        std::unique_ptr<Archive> archive = nullptr;

        ImGuiContext *imGuiContext;
        ImPlotContext *imPlotContext;

        float fpsLimit = 0;

        bool shutdown = false;

        virtual void start() {}

        virtual void stop() {}

        virtual void update(float deltaTime) {}
    };
}

#endif //MANA_APPLICATION_HPP
