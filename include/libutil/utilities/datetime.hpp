#pragma once

#include <chrono>

namespace dvsku::datetime {
    std::tm localtime(const time_t& time);
    std::tm localtime_now();
}