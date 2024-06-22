#pragma once

#include <fmt/format.h>
#include <fmt/chrono.h>

#include <string>
#include <vector>

namespace dvsku {
    #ifndef DV_FORMAT
    #define DV_FORMAT(frmt, ...)            \
        dvsku::util_string::format(frmt, ##__VA_ARGS__)
    #endif

    #ifndef DV_FORMAT_TO
    #define DV_FORMAT_TO(str, frmt, ...)    \
        dvsku::util_string::format_to(str, frmt, ##__VA_ARGS__)
    #endif

    class util_string {
    public:
        using string_view_t = fmt::string_view;

        /*
            Format a string.
        */
        template<typename... Targs>
        static std::string format(string_view_t format, Targs&&... args) {
            return fmt::format(fmt::runtime(format), args...);
        }

        template<typename... Targs>
        static void format_to(std::string& str, string_view_t format, Targs&&... args) {
            fmt::format_to(std::back_inserter(str), fmt::runtime(format), args...);
        }

        /*
            Split a string with a delimiter.
        */
        static std::vector<std::string> split(const std::string str, const char delimiter);
        
        /*
            Trim substring from front and back of a string.
        */
        static std::string trim(const std::string& str, const std::string& to_remove);

        /*
            Replace a substring with another in a string.
        */
        static std::string replace(const std::string& source, const std::string& what, const std::string& with);

        /*
            Check if a string contains a substring.
        */
        static bool contains(const std::string& haystack, const std::string& needle);
    };
}