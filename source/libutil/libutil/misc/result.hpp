#pragma once

#include <cstdint>
#include <string>

namespace libutil {
    /*
        Meant to be returned from what would be bool functions.
        Use to better describe the state.
    */
    struct result {
        int32_t     code    = -1;
        std::string message = "";

        explicit operator bool() {
            return is_success();
        }

        bool is_success() const {
            return code == 0;
        }

        bool is_cancelled() const {
            return code == 0xFFFFFFFF;
        }

        void success(const std::string& message = "") {
            code          = 0;
            this->message = message;
        }

        void cancel(const std::string& message = "") {
            code          = 0xFFFFFFFF;
            this->message = message;
        }

        void failure(int32_t code, const std::string& message = "") {
            code          = code;
            this->message = message;
        }
    };
}
