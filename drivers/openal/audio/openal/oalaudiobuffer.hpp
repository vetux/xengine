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

#ifndef XENGINE_OALAUDIOBUFFER_HPP
#define XENGINE_OALAUDIOBUFFER_HPP

#include "openal.hpp"

#include "audio/audiobuffer.hpp"

namespace xng {
    class OALAudioBuffer : public AudioBuffer {
    public:
        const ALuint handle;
        
        explicit OALAudioBuffer(ALuint handle);

        ~OALAudioBuffer() override;

        void upload(const std::vector<uint8_t> &buffer, AudioFormat format, unsigned int frequency) override;
    };
}

#endif //XENGINE_OALAUDIOBUFFER_HPP
