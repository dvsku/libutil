#include "libutil/utilities/string.hpp"

std::vector<std::string> libutil::string::split(const std::string str, const char delimiter) {
    std::vector<std::string> results;
    std::string::size_type   prev_pos = 0;
    std::string::size_type   pos      = 0;

    while ((pos = str.find(delimiter, pos)) != std::string::npos) {
        results.push_back(str.substr(prev_pos, pos - prev_pos));
        prev_pos = ++pos;
    }

    results.push_back(str.substr(prev_pos, pos - prev_pos));
    return results;
}

std::string libutil::string::trim(const std::string& str, const std::string& to_remove) {
    if (str.empty()) return str;

    size_t start = 0;
    while (start < str.size() && to_remove.find(str[start]) != std::string::npos) {
        start++;
    }

    size_t end = str.size();
    while (end > start && to_remove.find(str[end - 1]) != std::string::npos) {
        end--;
    }

    return str.substr(start, end - start);
}

std::string libutil::string::replace(const std::string& source, const std::string& what, const std::string& with) {
    std::string replaced = source;

    size_t start = 0;
    while ((start = replaced.find(what, start)) != std::string::npos) {
        replaced.replace(start, what.length(), with);
        start += with.length();
    }

    return replaced;
}

bool libutil::string::contains(const std::string& haystack, const std::string& needle) {
    return haystack.find(needle) != std::string::npos;
}
