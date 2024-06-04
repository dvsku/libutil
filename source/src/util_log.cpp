#include "libutil/util_log.hpp"
#include "libutil/util_datetime.hpp"
#include "libutil/util_string.hpp"
#include "libutil/config.hpp"

#ifdef DV_OS_WINDOWS
    #include <windows.h>
#endif

#include <fstream>
#include <iostream>

using namespace dvsku;

///////////////////////////////////////////////////////////////////////////////
// LOG

void util_log::init(const util_log::settings& settings) {
    if (m_impl) return;

    m_impl = std::make_unique<util_log::impl>();
    m_impl->settings = settings;
}

void util_log::create_source(const std::string& name, std::ostream* stream) {
    if (!m_impl)                        return;
    if (m_impl->sources.contains(name)) return;

    m_impl->sources.insert({ name, { true, stream } });
}

void util_log::remove_source(const std::string& name) {
    if (!m_impl)                         return;
    if (!m_impl->sources.contains(name)) return;

    m_impl->sources.erase(name);
}

util_log::source* util_log::get_source(const std::string& name) {
    if (!m_impl)                         return nullptr;
    if (!m_impl->sources.contains(name)) return nullptr;

    return &m_impl->sources[name];
}

util_log::settings* util_log::get_settings() {
    if (!m_impl) return nullptr;
    return &m_impl->settings;
}

///////////////////////////////////////////////////////////////////////////////
// LOG IMPL

void util_log::impl::log_to_sources(const std::string& message) const {
    for (auto& [name, source] : sources) {
        if (!source.enabled || !source.stream)
            continue;

    #ifdef DV_OS_WINDOWS
        if (source.stream == &std::cout) {
            auto handle = GetStdHandle(STD_OUTPUT_HANDLE);
            
            if (handle) {
                WriteConsole(handle, message.data(), (DWORD)message.size(), NULL, NULL);
            }

            return;
        }
    #endif  

        (*source.stream) << message;
    }
}

void util_log::impl::log_to_file(const std::string& message) const {
    if (!settings.log_to_file) return;

    try {
        auto localtime = util_datetime::localtime_now();

        std::string filename_format = settings.log_file_name;
        filename_format.append("_");
        filename_format.append(settings.log_file_timestamp_format);
        filename_format.append(".txt");

        std::string   filename = DV_FORMAT(filename_format, localtime);
        std::ofstream out(filename, std::ios_base::app);
        
        if (!out.is_open())
            return;

        out << message;
        out.close();
    }
    catch (...) {}
}
