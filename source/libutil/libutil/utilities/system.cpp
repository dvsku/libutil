#include "libutil/utilities/system.hpp"
#include "libutil/env/os.hpp"

#ifdef DV_OS_WINDOWS
    #include <windows.h>
#endif

std::filesystem::path libutil::system::get_executable_path() {
#ifdef DV_OS_WINDOWS
    CHAR  full_path[1024];
    DWORD full_path_size = RTL_NUMBER_OF(full_path);

    if (QueryFullProcessImageNameA(GetCurrentProcess(), 0, full_path, &full_path_size)) {
        std::filesystem::path path = std::filesystem::path(full_path);
        
        if (!path.is_absolute())
            path = std::filesystem::absolute(path);

        return std::filesystem::exists(path) ? path : "";
    }
#endif
    
    return "";
}

std::filesystem::path libutil::system::get_executable_dir() {
    std::filesystem::path filepath = get_executable_path();
    
    if (filepath == "")
        return "";

    filepath.remove_filename();
    return filepath;
}
