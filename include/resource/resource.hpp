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

#ifndef XENGINE_RESOURCE_HPP
#define XENGINE_RESOURCE_HPP

#include "algo/counter.hpp"

namespace xengine {
    class XENGINE_EXPORT Resource {
    public:
        /**
         * Each resource instance is identified with an Id value.
         */
        struct XENGINE_EXPORT Id {
            static Counter<size_t> counter;

            /**
             * Construct a new id,
             * ids are assigned from one global pool.
             */
            Id();

            /**
             * Deconstruct the id,
             * the id value is stored in the implementation and reused on a future call to the Id() constructor.
             */
            ~Id();

            size_t get() const { return value; }

            bool operator<(const Id &other) const {
                return value < other.value;
            }

        private:
            size_t value;
        };


        Resource() {}

        virtual ~Resource() = default;

        virtual Resource *clone() = 0;

        virtual Id getId() const { return id; }

    protected:
        Id id;
    };
}

#endif //XENGINE_RESOURCE_HPP
