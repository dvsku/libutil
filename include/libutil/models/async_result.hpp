#pragma once

#include "enums.hpp"

#include <string>

namespace dvsku {
    /*
        Meant to be returned from async functions via callbacks.
        Use to better describe the state.
    */
    struct async_result {
        async_result_status status  = async_result_status::failure;
        int32_t             code    = 0U;
        std::string         message = "";

        explicit operator bool() {
            return status == async_result_status::success;
        }
    };
}
