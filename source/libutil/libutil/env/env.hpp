#pragma once

#if defined(_WIN32) || defined(WIN32)
    #define DV_OS_WINDOWS
#else
    #define DV_OS_UNIX
#endif

#if !defined(NDEBUG)
    #define DV_DEBUG
#else
    #define DV_RELEASE
#endif
