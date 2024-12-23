#pragma once

#include <string>
#include <cstdint>
#include <functional>

namespace libutil {
    using uuid_t = uint64_t;

    namespace uuid {
        using unique_uuid_pred_t = std::function<bool(uuid_t)>;

        /*
            Create a uuid.
            Supply an optional predicate to keep creating until predicate returns true.
        */
        uuid_t create(unique_uuid_pred_t pred);

        /*
            Create a uuid from string.
        */
        uuid_t create(const std::string& str);

        /*
            Create a uuid from arbitrary data.
        */
        uuid_t create(const void* data, size_t size);
    }
}
