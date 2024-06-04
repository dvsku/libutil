#include "libutil/util_system.hpp"
#include "libutil/config.hpp"

#ifdef DV_OS_WINDOWS
    #include <windows.h>
#endif

using namespace dvsku;

std::string util_system::get_executable_file_path() {
#ifdef DV_OS_WINDOWS
    CHAR  filename[1024];
    DWORD filename_size = RTL_NUMBER_OF(filename);

    if (QueryFullProcessImageNameA(GetCurrentProcess(), 0, filename, &filename_size))
        return std::string(filename);
#endif

    return std::string();
}
