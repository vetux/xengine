/**
 *  xEngine - C++ Game Engine Library
 *  Copyright (C) 2023  Julian Zampiccoli
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

#ifndef XENGINE_SPIRVMODULE_HPP
#define XENGINE_SPIRVMODULE_HPP

#include <utility>
#include <vector>
#include <string>
#include <cstdint>

#include "shaderstage.hpp"
#include "spirvshader.hpp"
#include "spirvblob.hpp"

namespace xng {
    /**
     * A SPIRV bundle owns a set of SPIRVBlob objects and SPIRVShader objects referencing the blobs.
     */
    class SPIRVBundle {
    public:
        struct Entry {
            ShaderStage stage; // The stage of the shader
            std::string entryPoint; // The entry point of the shader
            size_t blobIndex; // The index of the blob containing the shader binary
        };

        SPIRVBundle() = default;

        SPIRVBundle(const std::vector<Entry> &entries, std::vector<SPIRVBlob> entryBlobs)
                : blobs(std::move(entryBlobs)) {
            for (auto &entry: entries) {
                modules.emplace_back(SPIRVShader(entry.stage, entry.entryPoint, blobs.at(entry.blobIndex)));
            }
        }

        const std::vector<SPIRVShader> &getShaders() const { return modules; }

        const SPIRVShader &getShader() const {
            if (modules.empty())
                throw std::runtime_error("No modules");
            return modules.at(0);
        }

        bool operator==(const SPIRVBundle &other) const {
            return modules == other.modules && blobs == other.blobs;
        }

    private:
        std::vector<SPIRVShader> modules;
        std::vector<SPIRVBlob> blobs;
    };
}

#endif //XENGINE_SPIRVMODULE_HPP
