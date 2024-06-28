#pragma once

#include <string>
#include <cstdint>

namespace libutil {
    using uuid = uint64_t;

    uuid create_uuid();
    uuid create_uuid(const std::string& str);
}