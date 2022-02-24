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

#ifndef XENGINE_TASK_HPP
#define XENGINE_TASK_HPP

#include <functional>
#include <condition_variable>
#include <mutex>
#include <atomic>

namespace xengine {
    class XENGINE_EXPORT Task {
    public:
        Task() : work(),
                 mutex(),
                 workDone(false),
                 workDoneCondition() {};

        Task(const Task &other) : work(other.work),
                                  mutex(),
                                  workDone(false),
                                  workDoneCondition() {}

        explicit Task(std::function<void()> work) : work(std::move(work)),
                                                    mutex(),
                                                    workDone(false),
                                                    workDoneCondition() {}

        Task &operator=(const Task &other);

        void start();

        const std::unique_ptr<std::exception> &wait();

        bool wait(long ms);

        /**
         * @return Nullptr if no exception was caught while work was invoked or a copy of an exception caught while work was invoked.
         */
        const std::unique_ptr<std::exception> &getException() { return exception; }

    private:
        std::unique_ptr<std::exception> exception; //If an exception was caught while the work was invoked this stores a pointer to a copy of the exception object.
        std::function<void()> work;
        std::mutex mutex;
        std::atomic<bool> workDone;
        std::condition_variable workDoneCondition;
    };
}

#endif //XENGINE_TASK_HPP
