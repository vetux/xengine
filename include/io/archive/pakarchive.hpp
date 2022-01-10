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

#ifndef XENGINE_PAKARCHIVE_HPP
#define XENGINE_PAKARCHIVE_HPP

#include <fstream>
#include <vector>
#include <mutex>

#include "io/archive.hpp"
#include "io/pak.hpp"

class XENGINE_EXPORT AssetPack;

namespace xengine {
    class XENGINE_EXPORT PakArchive : public Archive {
    public:
        PakArchive() = default;

        explicit PakArchive(std::vector<std::unique_ptr<std::istream>> stream,
                            bool verifyHashes = true,
                            const AES::Key &key = {},
                            const AES::InitializationVector &iv = {});

        ~PakArchive() override = default;

        bool exists(const std::string &path) override;

        std::unique_ptr<std::istream> open(const std::string &path) override;

    private:
        std::mutex mutex;
        Pak pak;
        bool verifyHashes;
    };
}

#endif //XENGINE_PAKARCHIVE_HPP
