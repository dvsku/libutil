#pragma once

#include <fmt/format.h>
#include <fmt/chrono.h>

#include <string>
#include <vector>

namespace libutil::string {
    #ifndef DV_FORMAT
    #define DV_FORMAT(frmt, ...)            \
        libutil::string::format(frmt, ##__VA_ARGS__)
    #endif

    #ifndef DV_FORMAT_TO
    #define DV_FORMAT_TO(str, frmt, ...)    \
        libutil::string::format_to(str, frmt, ##__VA_ARGS__)
    #endif

    /*
        Format a string.
    */
    template<typename... Targs>
    inline std::string format(fmt::string_view format, Targs&&... args) {
        return fmt::format(fmt::runtime(format), args...);
    }

    template<typename... Targs>
    inline void format_to(std::string& str, fmt::string_view format, Targs&&... args) {
        fmt::format_to(std::back_inserter(str), fmt::runtime(format), args...);
    }

    /*
        Split a string with a delimiter.
    */
    std::vector<std::string> split(const std::string str, const char delimiter);
    
    /*
        Trim substring from front and back of a string.
    */
    std::string trim(const std::string& str, const std::string& to_remove);

    /*
        Replace a substring with another in a string.
    */
    std::string replace(const std::string& source, const std::string& what, const std::string& with);

    /*
        Check if a string contains a substring.
    */
    bool contains(const std::string& haystack, const std::string& needle);
}