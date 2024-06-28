#include "libutil/utilities/datetime.hpp"

#include <fmt/chrono.h>

std::tm libutil::datetime::localtime(const time_t& time) {
    return fmt::localtime(time);
}

std::tm libutil::datetime::localtime_now() {
    return libutil::datetime::localtime(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
}
