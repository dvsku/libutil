#pragma once

#include <chrono>

namespace dvsku {
    class util_datetime {
    public:
        static std::tm localtime(const time_t& time);
        static std::tm localtime_now();
    };
}