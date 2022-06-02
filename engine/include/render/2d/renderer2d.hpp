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

#ifndef XENGINE_RENDERER2D_HPP
#define XENGINE_RENDERER2D_HPP

#include <set>
#include <unordered_set>

#include "graphics/renderer.hpp"
#include "graphics/renderdevice.hpp"

#include "render/shader/shadersource.hpp"

#include "asset/camera.hpp"

#include "text/text.hpp"

namespace xengine {
    /**
     * This is a SDL inspired 2d renderer.
     * It uses a similar interface to SDL in a object oriented manner.
     *
     * The 2d renderer uses the following coordinate system:
     *      -Y
     *       |
     * -X - -|- - +X
     *       |
     *      +Y
     *
     * with default boundary of 0,0 at the top left ond targetsize.x,targetsize.y at the bottom right.
     *
     * The Renderer2D ignores depth information stored in the render target.
     * The order in which methods are invoked controls which elements are drawn below / above.
     *
     * For example when drawing a rectangle texture first and then drawing a triangle texture afterwards
     * the triangle will be drawn on top of the rectangle.
     *
     * Drawing operations support blending between draw objects and previous target contents.
     *
     * Resources which are passed by the user (Texture buffers, shaders etc.) shall not be deallocated until
     * render finish is called.
     */
    class XENGINE_EXPORT Renderer2D {
    public:
        explicit Renderer2D(RenderDevice &device);

        ~Renderer2D();

        void renderBegin(RenderTarget &target, bool clear = true, ColorRGBA clearColor = ColorRGBA::black());

        void renderBegin(RenderTarget &target,
                         bool clear,
                         ColorRGBA clearColor,
                         Vec2i viewportOffset,
                         Vec2i viewportSize);

        /**
         * Set the projection bounds.
         * The projection bounds are set to 0,0 and target size when calling renderBegin
         *
         * @param projection The projection bounds to set
         */
        void setProjection(const Rectf &projection);

        /**
         * MVP variable is set on the shader program while this function is executing.
         *
         * @param srcRect
         * @param dstRect
         * @param texture
         * @param shader
         * @param center
         * @param rotation
         */
        void draw(Rectf srcRect,
                  Rectf dstRect,
                  TextureBuffer &texture,
                  ShaderProgram &shader,
                  Vec2f center = {},
                  float rotation = 0,
                  Vec2b flipUv = Vec2b(false),
                  bool alphaBlending = true);

        void draw(Rectf srcRect,
                  Rectf dstRect,
                  TextureBuffer &texture,
                  Vec2f center = {},
                  float rotation = 0,
                  Vec2b flipUv = Vec2b(false),
                  bool alphaBlending = true);

        void draw(Rectf dstRect, TextureBuffer &texture, Vec2f center = {}, float rotation = 0);

        void draw(Rectf rectangle,
                  ColorRGBA color,
                  bool fill = true,
                  Vec2f center = {},
                  float rotation = 0);

        void draw(Vec2f start, Vec2f end, ColorRGBA color, Vec2f center = {}, float rotation = 0);

        void draw(Vec2f point, ColorRGBA color = {});

        void draw(Text &text, Rectf dstRect, ColorRGBA color, Vec2f center = {}, float rotation = 0);

        void draw(Text &text, Rectf dstRect, ShaderProgram &shader, Vec2f center = {}, float rotation = 0);

        /**
         * @param offsets The position and rotation of every instance
         * @param size
         * @param color
         * @param fill
         * @param center
         */
        void drawInstanced(const std::vector<std::pair<Vec2f, float>> &offsets,
                           Vec2f size,
                           ColorRGBA color,
                           bool fill = true,
                           Vec2f center = {});

        void renderPresent();

    private:
        struct PlaneDescription {
            Vec2f size;
            Vec2f center;
            Rectf uvOffset;
            Vec2b flipUv;

            bool operator==(const PlaneDescription &other) const {
                return size == other.size
                       && center == other.center
                       && uvOffset == other.uvOffset
                       && flipUv == other.flipUv;
            }
        };

        class PlaneDescriptionHashFunction {
        public:
            size_t operator()(const PlaneDescription &p) const {
                size_t ret;
                hash_combine(ret, p.size.x);
                hash_combine(ret, p.size.y);
                hash_combine(ret, p.center.x);
                hash_combine(ret, p.center.y);
                hash_combine(ret, p.uvOffset.position.x);
                hash_combine(ret, p.uvOffset.position.y);
                hash_combine(ret, p.flipUv.x);
                hash_combine(ret, p.flipUv.y);
                return ret;
            }
        };

        struct SquareDescription {
            Vec2f size;
            Vec2f center;

            bool operator==(const SquareDescription &other) const {
                return size == other.size
                       && center == other.center;
            }
        };

        class SquareDescriptionHashFunction {
        public:
            size_t operator()(const SquareDescription &p) const {
                size_t ret;
                hash_combine(ret, p.size.x);
                hash_combine(ret, p.size.y);
                hash_combine(ret, p.center.x);
                hash_combine(ret, p.center.y);
                return ret;
            }
        };

        struct LineDescription {
            Vec2f start;
            Vec2f end;
            Vec2f center;

            bool operator==(const LineDescription &other) const {
                return start == other.start
                       && end == other.end
                       && center == other.center;
            }
        };

        class LineDescriptionHashFunction {
        public:
            size_t operator()(const LineDescription &p) const {
                size_t ret;
                hash_combine(ret, p.start.x);
                hash_combine(ret, p.start.y);
                hash_combine(ret, p.end.x);
                hash_combine(ret, p.end.y);
                hash_combine(ret, p.center.x);
                hash_combine(ret, p.center.y);
                return ret;
            }
        };

        template<typename T>
        class Vector2HashFunction {
        public:
            size_t operator()(const Vector2<T> &p) const {
                size_t ret;
                hash_combine(ret, p.x);
                hash_combine(ret, p.y);
                return ret;
            }
        };

        MeshBuffer &getPlane(const PlaneDescription &desc);

        MeshBuffer &getSquare(const SquareDescription &desc);

        MeshBuffer &getLine(const LineDescription &desc);

        MeshBuffer &getPoint(const Vec2f &point);

        RenderDevice &renderDevice;

        ShaderSource vs;
        ShaderSource fs;
        ShaderSource fsText;

        std::unique_ptr<ShaderProgram> defShader = nullptr;
        std::unique_ptr<ShaderProgram> textShader = nullptr;

        std::unordered_map<PlaneDescription, std::unique_ptr<MeshBuffer>, PlaneDescriptionHashFunction> allocatedPlanes;
        std::unordered_map<SquareDescription, std::unique_ptr<MeshBuffer>, SquareDescriptionHashFunction> allocatedSquares;
        std::unordered_map<LineDescription, std::unique_ptr<MeshBuffer>, LineDescriptionHashFunction> allocatedLines;
        std::unordered_map<Vec2f, std::unique_ptr<MeshBuffer>, Vector2HashFunction<float>> allocatedPoints;

        std::vector<std::unique_ptr<MeshBuffer>> allocatedInstancedMeshes;

        std::unordered_set<PlaneDescription, PlaneDescriptionHashFunction> usedPlanes;
        std::unordered_set<SquareDescription, SquareDescriptionHashFunction> usedSquares;
        std::unordered_set<LineDescription, LineDescriptionHashFunction> usedLines;
        std::unordered_set<Vec2f, Vector2HashFunction<float>> usedPoints;

        Vec2i screenSize;

        Camera camera;
    };
}

#endif //XENGINE_RENDERER2D_HPP