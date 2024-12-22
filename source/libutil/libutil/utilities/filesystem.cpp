#include "libutil/utilities/filesystem.hpp"
#include "libutil/utilities/system.hpp"

#include <string>

bool libutil::filesystem::is_sub_path(const std::filesystem::path& path, const std::filesystem::path& sub_path) {
    std::string path_str     = path.string();
    std::string sub_path_str = sub_path.string();

    return (path_str.find(sub_path_str) != std::string::npos);
}

std::filesystem::path libutil::filesystem::find_file(const std::filesystem::path& dir, const std::filesystem::path& file) {
    std::filesystem::path filepath = dir;
    filepath /= file;

    if (!filepath.is_absolute())
        filepath = std::filesystem::absolute(filepath);

    if (!std::filesystem::exists(filepath) || !std::filesystem::is_regular_file(filepath))
        return "";

    return filepath;
}

std::filesystem::path libutil::filesystem::find_file_recursive(const std::filesystem::path& dir, const std::filesystem::path& file) {
    if (dir == "" || file == "")
        return "";
    
    for (const auto& entry : std::filesystem::recursive_directory_iterator(dir)) {
        if (entry.is_regular_file() && entry.path().filename() == file) {
            std::filesystem::path filepath = entry.path();

            if (!filepath.is_absolute())
                filepath = std::filesystem::absolute(filepath);

            return filepath;
        }
    }

    return "";
}
