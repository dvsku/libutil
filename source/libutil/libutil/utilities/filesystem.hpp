#pragma once

#include <filesystem>

namespace libutil::filesystem {
    /*
        Check if a sub path is part of a path.
    */
    bool is_sub_path(const std::filesystem::path& path, const std::filesystem::path& sub_path);

    /*
        Get absolute path to a file in a directory.
    */
    std::filesystem::path find_file(const std::filesystem::path& dir, const std::filesystem::path& file);

    /*
        Get absolute path to a file in a directory or it's subdirectories.
    */
    std::filesystem::path find_file_recursive(const std::filesystem::path& dir, const std::filesystem::path& file);
}
