#pragma once

#include <stdexcept>

#define LIBUTIL_ASSERT(expr, message)   \
    if (!(expr)) throw std::runtime_error(message);
