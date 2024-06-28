#include "libutil/utilities/system.hpp"
#include "libutil/env/os.hpp"

#ifdef DV_OS_WINDOWS
    #include <windows.h>
#endif

std::string dvsku::system::get_executable_file_path() {
#ifdef DV_OS_WINDOWS
    CHAR  filename[1024];
    DWORD filename_size = RTL_NUMBER_OF(filename);

    if (QueryFullProcessImageNameA(GetCurrentProcess(), 0, filename, &filename_size))
        return std::string(filename);
#endif

    return std::string();
}
