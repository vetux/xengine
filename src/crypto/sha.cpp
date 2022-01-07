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

#include "crypto/sha.hpp"

#include "cryptopp/filters.h"
#include "cryptopp/cryptlib.h"
#include "cryptopp/sha.h"
#include "cryptopp/hex.h"

namespace engine{
    std::string SHA::sha256(const char *data, size_t length) {
        std::string tmp;
        std::string ret;
        CryptoPP::SHA256 hash;
        CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(ret));
        hash.Update((const CryptoPP::byte *) data, length);
        tmp.resize(hash.DigestSize());
        hash.Final((CryptoPP::byte *) &tmp[0]);
        CryptoPP::StringSource(tmp, true, new CryptoPP::Redirector(encoder));
        return ret;
    }

    std::string SHA::sha256(const std::string &data) {
        return sha256(data.data(), data.size());
    }

    std::string SHA::sha256(const std::vector<char> &data) {
        return sha256(data.data(), data.size());
    }
}