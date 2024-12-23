#pragma once

#include <memory>

namespace libutil::events {
    class event {
    public:
        using ptr_t = std::shared_ptr<event>;
    };

    template<typename T>
    concept is_event = !std::same_as<T, event> && std::derived_from<T, event>;

    class event_type {
    private:
        inline static uint64_t m_cnt = 0U;

    public:
        template<typename T>
        requires is_event<T>
        static const uint64_t id = m_cnt++;
    };
}
