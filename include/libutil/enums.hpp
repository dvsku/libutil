#pragma once

#include <cstdint>

namespace dvsku {
    enum class async_result_status : uint8_t {
        success,
        failure,
        cancel
    };
}
