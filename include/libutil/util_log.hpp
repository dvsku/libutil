#pragma once

#include "libutil/util_string.hpp"
#include "libutil/util_datetime.hpp"

#include <fmt/format.h>
#include <fmt/chrono.h>

#include <string>
#include <sstream>
#include <unordered_map>

#ifndef DV_LOG_INFO
#define DV_LOG_INFO(component, frmt, ...)     \
        dvsku::util_log::log_message(dvsku::util_log::level::informational, component, frmt, ##__VA_ARGS__)
#endif

#ifndef DV_LOG_WARN
#define DV_LOG_WARN(component, frmt, ...)     \
        dvsku::util_log::log_message(dvsku::util_log::level::warning,       component, frmt, ##__VA_ARGS__)
#endif

#ifndef DV_LOG_ERRO
#define DV_LOG_ERRO(component, frmt, ...)     \
        dvsku::util_log::log_message(dvsku::util_log::level::error,         component, frmt, ##__VA_ARGS__)
#endif

#ifndef DV_LOG_VERB
#define DV_LOG_VERB(component, frmt, ...)     \
        dvsku::util_log::log_message(dvsku::util_log::level::verbose,       component, frmt, ##__VA_ARGS__)
#endif

#ifndef DV_LOG_DEBG
#define DV_LOG_DEBG(component, frmt, ...)     \
        dvsku::util_log::log_message(dvsku::util_log::level::debug,         component, frmt, ##__VA_ARGS__)
#endif

namespace dvsku {
    class util_log {
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
            util_log::level level = util_log::level::error;
            
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

        struct source {
            /*
                Log to source?
            */
            bool enabled = true;

            /*
                Source stream
            */
            std::ostream* stream = nullptr;
        };

    public:
        static void init(const util_log::settings& settings);

        static                   void create_source(const std::string& name, std::ostream* stream);
        static                   void remove_source(const std::string& name);
        static   util_log::source* get_source(const std::string& name);
        static util_log::settings* get_settings();

        template<typename... Targs>
        static void log_message(util_log::level level, const std::string& component,
            string_view_t format, Targs&&... args)
        {
            if (!m_impl)                        return;
            if (m_impl->settings.level < level) return;

            bool log_to_sources = false;
            for (auto& [name, source] : m_impl->sources) {
                if (source.enabled) {
                    log_to_sources = true;
                    break;
                }
            }

            if (!m_impl->settings.log_to_file && !log_to_sources)
                return;

            std::stringstream ss;
            ss << "[";

            if (m_impl->settings.log_message_timestamp_format != "") {
                auto localtime = util_datetime::localtime_now();
                ss << util_string::format(m_impl->settings.log_message_timestamp_format, localtime);
                ss << " ";
            }

            ss << m_prefixes[(uint8_t)level];
            ss << "] ";

            if (component != "") {
                ss << util_string::format("[{}] ", component);
            }

            ss << util_string::format(format, std::forward<Targs>(args)...);
            ss << "\n";

            std::string message = ss.str();

            m_impl->log_to_sources(message);
            m_impl->log_to_file(message);
        }

    private:
        class impl {
        public:
            util_log::settings                                settings{};
            std::unordered_map<std::string, util_log::source> sources{};

        public:
            void log_to_sources(const std::string& message) const;
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

        inline static std::unique_ptr<util_log::impl> m_impl = nullptr;
    };
}