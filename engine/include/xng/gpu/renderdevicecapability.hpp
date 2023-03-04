/**
 *  This file is part of xEngine, a C++ game engine library.
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

#ifndef XENGINE_RENDERDEVICECAPABILITY_HPP
#define XENGINE_RENDERDEVICECAPABILITY_HPP

namespace xng {
    enum RenderDeviceCapability {
        RENDER_COMPUTE, // Support for compute pipelines
        RENDER_RAYTRACING, // Support for ray tracing pipelines
        RENDER_GPU_MEMORY, // Support for GpuMemory interface implementation

        RENDER_PIPELINE_MULTI_DRAW, // Support for multiDraw* methods of the RenderPipeline interface eg glMultiDraw on OpenGL
        RENDER_PIPELINE_INSTANCING, // Support for instancedDraw* methods of the RenderPipeline interface eg glDraw*Instanced on OpenGL
        RENDER_PIPELINE_BASE_VERTEX, // Support for *DrawIndexedBaseVertex methods of the RenderPipeline interface eg glDrawElements*BaseVertex on OpenGL
    };
}

#endif //XENGINE_RENDERDEVICECAPABILITY_HPP
