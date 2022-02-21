/**
 *  xEngine - C++ game engine library
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

#ifndef XENGINE_QTOGLRENDERER_HPP
#define XENGINE_QTOGLRENDERER_HPP

#include "graphics/renderer.hpp"

#include <QOpenGLFunctions_4_5_Core>

namespace xengine {
    namespace opengl {
        class QtOGLRenderer : public Renderer, public QOpenGLFunctions_4_5_Core {
        public:
            QtOGLRenderer() {
                QOpenGLFunctions_4_5_Core::initializeOpenGLFunctions();
            }

            void renderBegin(RenderTarget &target, const RenderOptions &options) override;

            void addCommand(RenderCommand &command) override;

            void renderFinish() override;

            void renderClear(RenderTarget &target, ColorRGBA color, float depth) override;

            void debugDrawCallRecordStart() override;

            unsigned long debugDrawCallRecordStop() override;

        private:
            unsigned long drawCalls = 0;
        };
    }
}

#endif //XENGINE_QTOGLRENDERER_HPP
