#pragma once

#include "libutil/env/assert.hpp"
#include "libutil/utilities/string.hpp"
#include "libutil/utilities/datetime.hpp"

#include <fmt/format.h>
#include <fmt/chrono.h>

#include <string>
#include <sstream>
#include <unordered_map>
#include <atomic>
#include <mutex>

#ifndef DV_LOG_INFO
#define DV_LOG_INFO(component, frmt, ...)     \
        libutil::log::log_message(libutil::log::level::informational, component, frmt, ##__VA_ARGS__)
#endif

#ifndef DV_LOG_WARN
#define DV_LOG_WARN(component, frmt, ...)     \
        libutil::log::log_message(libutil::log::level::warning,       component, frmt, ##__VA_ARGS__)
#endif

#ifndef DV_LOG_ERRO
#define DV_LOG_ERRO(component, frmt, ...)     \
        libutil::log::log_message(libutil::log::level::error,         component, frmt, ##__VA_ARGS__)
#endif

#ifndef DV_LOG_VERB
#define DV_LOG_VERB(component, frmt, ...)     \
        libutil::log::log_message(libutil::log::level::verbose,       component, frmt, ##__VA_ARGS__)
#endif

#ifndef DV_LOG_DEBG
#define DV_LOG_DEBG(component, frmt, ...)     \
        libutil::log::log_message(libutil::log::level::debug,         component, frmt, ##__VA_ARGS__)
#endif

namespace libutil {
    class log {
    public:
        using string_view_t = fmt::string_view;

    public:
        enum class level : unsigned char {
            none          = 0x00,   // does not log
            informational = 0x01,   // logs only info
            warning       = 0x02,   // logs info and warnings
            error         = 0x03,   // logs info, warnings and errors
            verbose       = 0x04,   // logs info, warnings, errors and verbose
            debug         = 0x05    // logs everything + debug
        };

        struct settings {
            /*
                Log level
            */
            log::level level = log::level::error;
            
            /*
                Log to console?
            */
            bool log_to_console = true;

            /*
                Log to file?
            */
            bool log_to_file = false;

            /*
                Log message timestamp format
            */
            std::string log_message_timestamp_format = "{:%H:%M:%S}";

            /*
                Log file name
            */
            std::string log_file_name = "log";

            /*
                Log file timestamp format
            */
            std::string log_file_timestamp_format = "{:%Y_%m_%d}";

            /*
                Log files dir
                Will be created if it doesn't exist
            */
            std::string log_files_dir = "logs";
        };

    public:
        log() = delete;

    public:
        static void init(const log::settings& settings);

        /*
            Pause logging.
        */
        static void pause();

        /*
            Resume logging.
        */
        static void resume();

        /*
            Create a stream to log into.
        */
        static void create_stream(const std::string& name);

        /*
            Remove stream.
        */
        static void remove_stream(const std::string& name);

        /*
            Enable/disable stream.
        */
        static void update_stream_state(const std::string& name, bool enabled);

        /*
            Clear stream.
        */
        static void clear_stream(const std::string& name);

        /*
            Get stream content.
        */
        static std::string get_stream_content(const std::string& name);

        /*
            Get log settings.
            Log must be paused before changing settings.
        */
        static log::settings* get_settings();

        template<typename... Targs>
        static void log_message(log::level level, const std::string& component,
            string_view_t format, Targs&&... args)
        {
            LIBUTIL_ASSERT(m_impl, "Log not initialized.");

            if (!m_running)                     return;
            if (m_impl->settings.level < level) return;

            try {
                std::lock_guard guard(m_mutex);

                bool log_to_streams = false;
                for (auto& [name, map_stream] : m_impl->streams) {
                    if (map_stream.enabled) {
                        log_to_streams = true;
                        break;
                    }
                }

                if (!m_impl->settings.log_to_file && !m_impl->settings.log_to_console && !log_to_streams)
                    return;

                std::stringstream ss;
                ss << "[";

                if (m_impl->settings.log_message_timestamp_format != "") {
                    auto localtime = datetime::localtime_now();
                    ss << string::format(m_impl->settings.log_message_timestamp_format, localtime);
                    ss << " ";
                }

                ss << m_prefixes[(uint8_t)level];
                ss << "] ";

                if (component != "") {
                    ss << string::format("[{}] ", component);
                }

                ss << string::format(format, std::forward<Targs>(args)...);
                ss << "\n";

                std::string message = ss.str();

                m_impl->log_to_file(message);
                m_impl->log_to_streams(message);
            }
            catch(...) {}
        }

    private:
        struct stream {
            bool              enabled = true;
            std::stringstream stream;
        };

        class impl {
        public:
            log::settings                                settings{};
            std::unordered_map<std::string, log::stream> streams{};

        public:
            void log_to_streams(const std::string& message);
            void log_to_file(const std::string& message) const;
        };

    private:
        inline static const char* m_prefixes[] = {
            {""},
            {"INFO"},
            {"WARN"},
            {"ERRO"},
            {"VERB"},
            {"DEBG"}
        };

        inline static std::unique_ptr<log::impl> m_impl    = nullptr;
        inline static std::atomic_bool           m_running = false;
        inline static std::mutex                 m_mutex;
    };
}