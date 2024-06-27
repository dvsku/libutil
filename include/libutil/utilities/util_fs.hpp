#pragma once

#include <string>
#include <filesystem>

namespace dvsku {
    class util_fs {
    public:
        /*
            Check if a sub path is part of a path
        */
        static bool is_sub_path(const std::filesystem::path& path, const std::filesystem::path& sub_path);

        /*
            Get absolute file path from exe base dir
        */
        static std::filesystem::path exe_base_find_file(const std::string& file);

        /*
            Get absolute file dir from the exe base dir
        */
        static std::filesystem::path exe_base_find_file_dir(const std::string& file);

        /*
            Check if file exists in exe base dir
        */
        static bool exe_base_exists(const std::string& file);
    };
}