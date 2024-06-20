#pragma once

#include <cstdint>
#include <string>

namespace dvsku {
    /*
        Meant to be returned from what would be bool functions.
        Use to better describe the state.
    */
    struct result {
        bool        success = false;
        int32_t     code    = 0U;
        std::string message = "";

        explicit operator bool() {
            return success;
        }
    };
}
