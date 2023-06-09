/**
 *  xEngine - C++ game engine library
 *  Copyright (C) 2023  Julian Zampiccoli
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

#ifndef XENGINE_OGLCOMMANDBUFFER_HPP
#define XENGINE_OGLCOMMANDBUFFER_HPP

#include "xng/gpu/commandbuffer.hpp"

#include <utility>

namespace xng {
    namespace opengl {
        class OGLCommandBuffer : public CommandBuffer {
        public:
            std::function<void(RenderObject * )> destructor;

            std::vector<Command> commands;

            explicit OGLCommandBuffer(std::function<void(RenderObject * )> destructor) : destructor(
                    std::move(destructor)) {}

            ~OGLCommandBuffer() {
                destructor(this);
            }

            Type getType() override {
                return RENDER_OBJECT_COMMAND_BUFFER;
            }

            void begin() override {
                commands.clear();
            }

            CommandBuffer &add(const std::vector<Command> &commandsArg) override {
                commands.insert(commands.end(), commandsArg.begin(), commandsArg.end());
                return *this;
            }

            void end() override {}
        };
    }
}

#endif //XENGINE_OGLCOMMANDBUFFER_HPP
