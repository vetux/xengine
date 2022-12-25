/**
 *  This file is part of xEngine, a C++ game engine library.
 *  Copyright (C) 2022  Julian Zampiccoli
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

#ifndef XENGINE_ENTITYRUNTIME_HPP
#define XENGINE_ENTITYRUNTIME_HPP

#include <set>
#include <memory>

#include "system.hpp"
#include "entity.hpp"
#include "xng/ecs/profiling/ecsprofiler.hpp"

#include "xng/event/eventbus.hpp"

#include "xng/types/deltatime.hpp"

namespace xng {
    class XENGINE_EXPORT EntityRuntime {
    public:
        explicit EntityRuntime(std::vector<std::reference_wrapper<System>> systems = {}, std::shared_ptr<EntityScene> scene = {});

        ~EntityRuntime();

        EntityRuntime(EntityRuntime &&other) noexcept;

        EntityRuntime &operator=(EntityRuntime &&other) noexcept;

        void start();

        void update(DeltaTime deltaTime);

        void stop();

        const std::shared_ptr<EntityScene> &getScene() const;

        void setScene(const std::shared_ptr<EntityScene> &scene);

        const std::vector<std::reference_wrapper<System>> &getSystems() const;

        void setSystems(const std::vector<std::reference_wrapper<System>> &systems);

        void setEnableProfiling(bool value) {
            enableProfiling = value;
        }

        const ECSFrameList &getFrameList() const;

        void setEventBus(const std::shared_ptr<EventBus> &ptr) {
            bool reset = started;
            if (started) {
                stop();
            }
            if (ptr) {
                eventBus = ptr;
            } else {
                eventBus = std::make_shared<EventBus>();
            }
            if (reset){
                start();
            }
        }

        std::shared_ptr<EventBus> getEventBus() const {
            return eventBus;
        }

    private:
        bool enableProfiling = false;
        ECSProfiler profiler;
        bool started = false;
        std::shared_ptr<EntityScene> scene;
        std::vector<std::reference_wrapper<System>> systems;
        std::shared_ptr<EventBus> eventBus;
    };
}

#endif //XENGINE_ENTITYRUNTIME_HPP
