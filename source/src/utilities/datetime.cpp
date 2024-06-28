#include "libutil/utilities/datetime.hpp"

#include <fmt/chrono.h>

std::tm dvsku::datetime::localtime(const time_t& time) {
    return fmt::localtime(time);
}

std::tm dvsku::datetime::localtime_now() {
    return dvsku::datetime::localtime(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
}
