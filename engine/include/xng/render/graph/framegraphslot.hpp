/**
 *  xEngine - C++ Game Engine Library
 *  Copyright (C) 2023  Julian Zampiccoli
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef XENGINE_FRAMEGRAPHSLOT_HPP
#define XENGINE_FRAMEGRAPHSLOT_HPP

namespace xng {
    enum FrameGraphSlot : int {
        // Color and Depth Textures, Created and cleared by the clear pass
        SLOT_SCREEN_COLOR = 0, // The combined deferred and forward colors from the compositing pass, can be written to by post processing or overlay (eg. gui) passes.
        SLOT_SCREEN_DEPTH, // The combined deferred and forward screen depth from the compositing pass

        SLOT_DEFERRED_COLOR, // The combined color of the deferred rendered objects, can be written to by post processing passes which require separated transparency and color textures (DOF).
        SLOT_DEFERRED_DEPTH, // The depth of the deferred rendered objects

        SLOT_FORWARD_COLOR, // The combined color of the forward rendered objects (Transparency)
        SLOT_FORWARD_DEPTH, // The depth of the forward rendered objects

        SLOT_BACKGROUND_COLOR, // The non-transparent color of the background (eg. Skybox), the compositing pass blits the deferred color over this based on the depth and then blends the forward color ontop and stores the result in screen color.

        // The GBuffer Textures, Created and assigned by the construction pass
        SLOT_GBUFFER_POSITION,          // Texture RGBA32F : World Space Position xyz, w = X
        SLOT_GBUFFER_NORMAL,                // Texture RGBA32F : Vertex or Texture Normal xyz, w = X
        SLOT_GBUFFER_TANGENT,               // Texture RGBA32F : Vertex Tangent xyz, w = X
        SLOT_GBUFFER_ROUGHNESS_METALLIC_AO, // Texture RGBA32F : .x = roughness, .y = metallic, .z = ambient occlusion
        SLOT_GBUFFER_ALBEDO,                // Texture RGBA : xyzw = Albedo
        SLOT_GBUFFER_OBJECT_SHADOWS,          // Texture RGBA32I : .x = Object ID, .y = Receive Shadows
        SLOT_GBUFFER_DEPTH,                 // Texture DEPTH_STENCIL : The depth value in the x component

        // Shadow Maps
        SLOT_SHADOW_MAP_POINT, // A Texture Array with 3d cubemap textures containing point light depth maps of light sources.

        // User slots are not used by the default passes included with xng and can be used to share data between user created passes.
        SLOT_USER_1 = 255,
        SLOT_USER_2,
        SLOT_USER_3,
        SLOT_USER_4,
        SLOT_USER_5,
        SLOT_USER_6,
        SLOT_USER_7,
        SLOT_USER_8,
        SLOT_USER_9,
        SLOT_USER_10,
        SLOT_USER_11,
        SLOT_USER_12,
        SLOT_USER_13,
        SLOT_USER_14,
        SLOT_USER_15,
        SLOT_USER_16,
        SLOT_USER_17,
        SLOT_USER_18,
        SLOT_USER_19,
        SLOT_USER_20,
    };
}

#endif //XENGINE_FRAMEGRAPHSLOT_HPP
