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

#ifndef MANA_AUDIOCONTEXT_HPP
#define MANA_AUDIOCONTEXT_HPP

#include <memory>

#include "audiolistener.hpp"
#include "audiobuffer.hpp"
#include "audiosource.hpp"

namespace engine {
    class MANA_EXPORT AudioContext {
    public:
        virtual ~AudioContext() = default;

        virtual void makeCurrent() = 0;

        virtual AudioListener &getListener() = 0;

        virtual std::unique_ptr<AudioBuffer> createBuffer() = 0;

        virtual std::unique_ptr<AudioSource> createSource() = 0;
    };
}

#endif //MANA_AUDIOCONTEXT_HPP
