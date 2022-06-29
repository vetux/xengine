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

#ifndef XENGINE_COLOR_HPP
#define XENGINE_COLOR_HPP

#include <cstdint>

#include "math/vector4.hpp"

#include "cast/numeric_cast.hpp"

namespace xng {
    struct XENGINE_EXPORT ColorRGB {
        uint8_t data[3];

        uint8_t &r() { return data[0]; }

        uint8_t &g() { return data[1]; }

        uint8_t &b() { return data[2]; }

        const uint8_t &r() const { return data[0]; }

        const uint8_t &g() const { return data[1]; }

        const uint8_t &b() const { return data[2]; }

        ColorRGB() : data() {}

        ColorRGB(uint8_t r, uint8_t g, uint8_t b)
                : data() {
            this->r() = r;
            this->g() = g;
            this->b() = b;
        }

        explicit ColorRGB(uint8_t v)
                : data() {
            this->r() = v;
            this->g() = v;
            this->b() = v;
        }

        bool operator==(const ColorRGB &other) const {
            return r() == other.r() && g() == other.g() && b() == other.b();
        }

        bool operator!=(const ColorRGB &other) const {
            return !(*this == other);
        }
    };

    struct XENGINE_EXPORT ColorRGBA {
        uint8_t data[4];

        static ColorRGBA black(float intensity = 1, uint8_t alpha = 255) {
            return {
                    0,
                    0,
                    0,
                    alpha
            };
        }

        static ColorRGBA white(float intensity = 1, uint8_t alpha = 255) {
            return {
                    numeric_cast<uint8_t>(255 * intensity),
                    numeric_cast<uint8_t>(255 * intensity),
                    numeric_cast<uint8_t>(255 * intensity),
                    alpha
            };
        }

        static ColorRGBA grey(float intensity = 1, uint8_t alpha = 255) {
            return {
                    numeric_cast<uint8_t>(64 * intensity),
                    numeric_cast<uint8_t>(64 * intensity),
                    numeric_cast<uint8_t>(64 * intensity),
                    alpha
            };
        }

        static ColorRGBA red(float intensity = 1, uint8_t alpha = 255) {
            return {
                    numeric_cast<uint8_t>(255 * intensity),
                    numeric_cast<uint8_t>(5 * intensity),
                    numeric_cast<uint8_t>(5 * intensity),
                    alpha
            };
        }

        static ColorRGBA green(float intensity = 1, uint8_t alpha = 255) {
            return {
                    numeric_cast<uint8_t>(5 * intensity),
                    numeric_cast<uint8_t>(255 * intensity),
                    numeric_cast<uint8_t>(5 * intensity),
                    alpha
            };
        }

        static ColorRGBA blue(float intensity = 1, uint8_t alpha = 255) {
            return {
                    numeric_cast<uint8_t>(5 * intensity),
                    numeric_cast<uint8_t>(5 * intensity),
                    numeric_cast<uint8_t>(255 * intensity),
                    alpha
            };
        }

        static ColorRGBA yellow(float intensity = 1, uint8_t alpha = 255) {
            return {
                    numeric_cast<uint8_t>(255 * intensity),
                    numeric_cast<uint8_t>(255 * intensity),
                    numeric_cast<uint8_t>(5 * intensity),
                    alpha
            };
        }

        static ColorRGBA cyan(float intensity = 1, uint8_t alpha = 255) {
            return {
                    numeric_cast<uint8_t>(5 * intensity),
                    numeric_cast<uint8_t>(255 * intensity),
                    numeric_cast<uint8_t>(255 * intensity),
                    alpha
            };
        }

        uint8_t &r() { return data[0]; }

        uint8_t &g() { return data[1]; }

        uint8_t &b() { return data[2]; }

        uint8_t &a() { return data[3]; }

        const uint8_t &r() const { return data[0]; }

        const uint8_t &g() const { return data[1]; }

        const uint8_t &b() const { return data[2]; }

        const uint8_t &a() const { return data[3]; }

        ColorRGBA() : data() {}

        ColorRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
                : data() {
            this->r() = r;
            this->g() = g;
            this->b() = b;
            this->a() = a;
        }

        explicit ColorRGBA(uint8_t v)
                : data() {
            this->r() = v;
            this->g() = v;
            this->b() = v;
            this->a() = v;
        }

        bool operator==(const ColorRGBA &other) const {
            return r() == other.r()
                   && g() == other.g()
                   && b() == other.b()
                   && a() == other.a();
        }

        bool operator!=(const ColorRGBA &other) const {
            return !(*this == other);
        }

        Vec4f divide(float divisor = 255) const {
            return {static_cast<float>(r()) / divisor,
                    static_cast<float>(g()) / divisor,
                    static_cast<float>(b()) / divisor,
                    static_cast<float>(a()) / divisor};
        }
    };
}

#endif //XENGINE_COLOR_HPP
