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

#ifndef MANA_INPUT_HPP
#define MANA_INPUT_HPP

#include <set>

#include "inputlistener.hpp"

#include "asset/image.hpp"

namespace engine {
    class MANA_EXPORT Input {
    public:
        virtual ~Input() = default;

        virtual void addListener(InputListener &listener) = 0;

        virtual void removeListener(InputListener &listener) = 0;

        virtual void setClipboardText(std::string text) = 0;

        virtual std::string getClipboardText() = 0;

        virtual void setMouseCursorImage(const Image<ColorRGBA> &image) = 0;

        virtual void clearMouseCursorImage() = 0;

        virtual const std::map<int, Keyboard> &getKeyboards() const = 0;

        virtual const std::map<int, Mouse> &getMice() const = 0;

        virtual const std::map<int, GamePad> &getGamePads() const = 0;
    };
}

#endif //MANA_INPUT_HPP
