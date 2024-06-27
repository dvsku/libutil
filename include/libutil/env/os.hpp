#pragma once

#if defined(_WIN32) || defined(WIN32)
    #define DV_OS_WINDOWS
#else
    #define DV_OS_UNIX
#endif
