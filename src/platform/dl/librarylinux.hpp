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

#ifndef MANA_LIBRARYLINUX_HPP
#define MANA_LIBRARYLINUX_HPP

#include "platform/dl/library.hpp"

#include <string>
#include <dlfcn.h>

namespace engine {
    class LibraryLinux : public Library {
    public:
        explicit LibraryLinux(const std::string &filePath)
                : filePath(filePath) {
            handle = dlopen(filePath.c_str(), RTLD_LAZY);
        }

        ~LibraryLinux() override {
            dlclose(handle);
        }

        void *getSymbolAddress(const std::string &address) override {
            return dlsym(handle, address.c_str());
        }

        std::string filePath;
        void *handle;
    };

    typedef LibraryLinux LibraryOS;
}

#endif //MANA_LIBRARYLINUX_HPP
