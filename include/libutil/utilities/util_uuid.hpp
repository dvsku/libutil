#pragma once

#include <string>
#include <cstdint>

namespace dvsku {
    using uuid = uint64_t;

    struct util_uuid {
        static uuid create();
        static uuid create(const std::string& str);
    };
}