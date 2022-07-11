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

#ifndef XENGINE_SNDFILEPARSER_HPP
#define XENGINE_SNDFILEPARSER_HPP

#include "resource/resourceparser.hpp"

namespace xng {
    class XENGINE_EXPORT SndFileParser : public ResourceParser {
    public:
        ResourceBundle parse(const std::string &buffer,
                             const std::string &hint,
                             const ResourceImporter &importer,
                             Archive *archive) const override;

        const std::set<std::string> &getSupportedFormats() const override;
    };
}

#endif //XENGINE_SNDFILEPARSER_HPP