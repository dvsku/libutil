#pragma once

#include <string>
#include <cstdint>

namespace dvsku {
    using uuid = uint64_t;

    uuid create_uuid();
    uuid create_uuid(const std::string& str);
}