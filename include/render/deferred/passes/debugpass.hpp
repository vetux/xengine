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

#ifndef MANA_DEBUGPASS_HPP
#define MANA_DEBUGPASS_HPP

#include "render/deferred/renderpass.hpp"

namespace engine {
    class MANA_EXPORT DebugPass : public RenderPass {
    public:
        static const char *WIREFRAME;
        static const char *LIGHTS;
        static const char *NORMALS;

        explicit DebugPass(RenderDevice &device);

        ~DebugPass() override;

        void prepareBuffer(GeometryBuffer &gBuffer) override;

        void render(GeometryBuffer &gBuffer, Scene &scene, AssetRenderManager &assetRenderManager) override;

    private:
        RenderDevice &device;

        ShaderSource vs;
        ShaderSource gs;
        ShaderSource fs;

        ShaderSource vsl;
        ShaderSource gsl;

        ShaderSource vsw;
        ShaderSource gsw;

        std::unique_ptr<ShaderProgram> shaderWireframe;
        std::unique_ptr<ShaderProgram> shaderNormals;
        std::unique_ptr<ShaderProgram> shaderLight;
        std::unique_ptr<MeshBuffer> meshBuffer;
    };
}

#endif //MANA_DEBUGPASS_HPP
