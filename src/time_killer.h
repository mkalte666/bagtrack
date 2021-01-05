// licence note at the end of the file
#ifndef BAGTRACK_TIME_KILLER_H
#define BAGTRACK_TIME_KILLER_H

#include <chrono>
#include <condition_variable>
#include <mutex>

/**
 * \brief Stolen from https://stackoverflow.com/questions/29775153/stopping-long-sleep-threads
 */
struct timer_killer {
    // returns false if killed:
    template <class R, class P>
    bool wait_for(std::chrono::duration<R, P> const& time) const
    {
        std::unique_lock<std::mutex> lock(m);
        return !cv.wait_for(lock, time, [&] { return terminate; });
    }
    void kill()
    {
        std::unique_lock<std::mutex> lock(m);
        terminate = true; // should be modified inside mutex lock
        cv.notify_all(); // it is safe, and *sometimes* optimal, to do this outside the lock
    }
    // I like to explicitly delete/default special member functions:
    timer_killer() = default;
    ~timer_killer() = default;
    timer_killer(timer_killer&&) = delete;
    timer_killer(timer_killer const&) = delete;
    timer_killer& operator=(timer_killer&&) = delete;
    timer_killer& operator=(timer_killer const&) = delete;

private:
    mutable std::condition_variable cv = {};
    mutable std::mutex m = {};
    bool terminate = false;
};

#endif //BAGTRACK_TIME_KILLER_H
/*
 * This file is part bagtrack
 * Copyright (c) 2020 Malte Kießling
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */