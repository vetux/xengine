/**
 *  This file is part of xEngine, a C++ game engine library.
 *  Copyright (C) 2021  Julian Zampiccoli
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

#ifndef XENGINE_LISTENABLE_HPP
#define XENGINE_LISTENABLE_HPP

#include <functional>

namespace xng {
    template<typename T>
    class Listenable {
    public:
        typedef std::function<void()> UnregisterCallback;

        virtual ~Listenable() = default;

        /**
         * @param listener
         * @return A function object which when invoked calls removeListener with the passed listener on this Listenable.
         */
        virtual UnregisterCallback addListener(T &listener) = 0;

        virtual void removeListener(T &listener) = 0;
    };
}
#endif //XENGINE_LISTENABLE_HPP
