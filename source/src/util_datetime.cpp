#include "libutil/util_datetime.hpp"

#include <fmt/chrono.h>

using namespace dvsku;

std::tm dv_util_datetime::localtime(const time_t& time) {
    return fmt::localtime(time);
}

std::tm dv_util_datetime::localtime_now() {
    return dv_util_datetime::localtime(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
}
