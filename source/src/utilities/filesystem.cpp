#include "libutil/utilities/filesystem.hpp"
#include "libutil/utilities/system.hpp"

#include <string>

bool libutil::filesystem::is_sub_path(const std::filesystem::path& path, const std::filesystem::path& sub_path) {
    std::string path_str     = path.string();
    std::string sub_path_str = sub_path.string();

    return (path_str.find(sub_path_str) != std::string::npos);
}

std::filesystem::path libutil::filesystem::exe_dir_find_file(const std::string& file) {
    std::filesystem::path filepath = exe_dir_find_file_dir(file);
    if (filepath.empty())
        return std::filesystem::path();

    filepath.append(file);
    return filepath;
}

std::filesystem::path libutil::filesystem::exe_dir_find_file_dir(const std::string& file) {
    std::filesystem::path exe = libutil::system::get_executable_file_path();
    exe.remove_filename();

    if (exe.empty())
        return std::filesystem::path();

    for (const auto& entry : std::filesystem::recursive_directory_iterator(exe)) {
        if (entry.is_regular_file() && entry.path().filename() == file) {
            std::filesystem::path filedir = entry.path();
            filedir.remove_filename();

            return filedir;
        }
    }

    return std::filesystem::path();
}

bool libutil::filesystem::exe_dir_file_exists(const std::string& file) {
    auto filepath = exe_dir_find_file(file);
    
    if (filepath.empty())
        return false;

    return std::filesystem::exists(filepath);
}
