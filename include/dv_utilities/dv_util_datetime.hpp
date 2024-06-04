#pragma once

#include <chrono>

namespace dvsku {
    class dv_util_datetime {
    public:
        static std::tm localtime(const time_t& time);
        static std::tm localtime_now();
    };
}