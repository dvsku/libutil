#pragma once

#include <filesystem>

namespace libutil::system {
    /*
        Get absolute path to current executable file.
    */
    std::filesystem::path get_executable_path();

    /*
        Get absolute path to directory of current executable file.
    */
    std::filesystem::path get_executable_dir();
}