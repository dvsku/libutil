#pragma once

#include <chrono>

namespace libutil::datetime {
    std::tm localtime(const time_t& time);
    std::tm localtime_now();
}