#pragma once

#include "libutil/event/event.hpp"

#include <eventpp/eventqueue.h>
#include <cstdint>

namespace libutil::events {
    class event_system {
    public:
        using event_t       = libutil::events::event;
        using event_queue_t = eventpp::EventQueue<uint64_t, void(const event_t::ptr_t)>;
        using handle_t      = event_queue_t::Handle;

    public:
        template<typename T>
        requires is_event<T>
        void enqueue(T&& event) {
            m_queue.enqueue(events::event_type::id<T>, std::make_shared<T>(std::move(event)));
        }

        template<typename T>
        requires is_event<T>
        handle_t add_listener(std::function<void(const T&)> callback) {
            return m_queue.appendListener(events::event_type::id<T>, [callback](const event_t::ptr_t event) {
                callback(*std::static_pointer_cast<T>(event));
            });
        }

        template<typename T>
        requires is_event<T>
        void remove_listener(handle_t handle) {
            m_queue.removeListener(events::event_type::id<T>, handle);
        }

        void process() {
            m_queue.process();
        }

        void process_one() {
            m_queue.processOne();
        }

    private:
        event_queue_t m_queue;
    };
}
