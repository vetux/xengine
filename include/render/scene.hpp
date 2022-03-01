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

#ifndef XENGINE_SCENE_HPP
#define XENGINE_SCENE_HPP

#include "camera.hpp"
#include "light.hpp"
#include "asset/assetmaterial.hpp"
#include "asset/assetskybox.hpp"
#include "asset/shader.hpp"
#include "asset/assethandle.hpp"

#include "render/material.hpp"
#include "render/skybox.hpp"

#include "render/platform/rendercommand.hpp"

namespace xengine {
    struct XENGINE_EXPORT Scene {
        struct XENGINE_EXPORT Node {
            Node() = default;

            Node(Transform t, MeshBuffer *mesh, Material material)
                    : transform(t), mesh(mesh), material(material) {}

            Transform transform;

            MeshBuffer *mesh = nullptr;
            Material material;

            bool outline = false;
            ColorRGBA outlineColor;
            float outlineScale = 1.1f;

            bool castShadow = false;
            bool receiveShadow = false;
        };

        Camera camera;
        Skybox skybox;
        std::vector<Light> lights;
        std::vector<Node> nodes;
    };
}

#endif //XENGINE_SCENE_HPP
