/*
Copyright (C) 2005 StrmnNrmn

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include <chrono>
#include <cstdint>

#include "Base/Types.h"
namespace NTiming {

    bool GetPreciseFrequency(u64 *p_freq) {
        *p_freq = 1'000'000;  // Microseconds
        return true;
    }

    bool GetPreciseTime(u64 *p_time) {
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());
        *p_time = static_cast<u64>(duration.count());
        return true;
    }

    u64 ToMilliseconds(u64 ticks) {
        return (ticks * 1'000LL) / 1'000'000LL;
    }
} // NTiming
