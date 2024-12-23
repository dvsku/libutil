#pragma once

#include "libutil/env/env.hpp"

#if defined(DV_OS_WINDOWS) && defined(DV_DEBUG)
    #include <windows.h>
    #include <stdexcept>

    #define LIBUTIL_ASSERT(statement, message)                                  \
        if (!(statement)) {                                                     \
            MessageBox(NULL, message, "Assert fail", MB_OK | MB_ICONERROR);     \
            throw std::runtime_error(message);                                  \
        }
#elif defined(DV_DEBUG)
    #include <stdexcept>

    #define LIBUTIL_ASSERT(statement, message)                                  \
        if (!(statement)) {                                                     \
            throw std::runtime_error(message);                                  \
        }
#else
    #define LIBUTIL_ASSERT(statement, message) do {} while(0)
#endif
