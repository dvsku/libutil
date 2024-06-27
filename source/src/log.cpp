#include "libutil/log.hpp"
#include "libutil/utilities/util_datetime.hpp"
#include "libutil/utilities/util_string.hpp"
#include "libutil/env/os.hpp"

#ifdef DV_OS_WINDOWS
    #include <windows.h>
#endif

#include <fstream>
#include <iostream>

using namespace dvsku;

///////////////////////////////////////////////////////////////////////////////
// LOG

void log::init(const log::settings& settings) {
    if (m_impl) return;

    m_impl = std::make_unique<log::impl>();
    m_impl->settings = settings;

    resume();
}

void log::pause() {
    m_running = false;
}

void log::resume() {
    m_running = true;
}

void log::create_stream(const std::string& name) {
    LIBUTIL_ASSERT(m_impl, "Log not initialized.");
    
    std::lock_guard guard(m_mutex);

    if (m_impl->streams.contains(name))
        return;

    m_impl->streams.insert({ name, log::stream() });
}

void log::remove_stream(const std::string& name) {
    LIBUTIL_ASSERT(m_impl, "Log not initialized.");
    
    std::lock_guard guard(m_mutex);

    if (!m_impl->streams.contains(name))
        return;

    m_impl->streams.erase(name);
}

void log::update_stream_state(const std::string& name, bool enabled) {
    LIBUTIL_ASSERT(m_impl, "Log not initialized.");

    std::lock_guard guard(m_mutex);

    if (!m_impl->streams.contains(name))
        return;

    m_impl->streams[name].enabled = enabled;
}

void log::clear_stream(const std::string& name) {
    LIBUTIL_ASSERT(m_impl, "Log not initialized.");

    std::lock_guard guard(m_mutex);

    if (!m_impl->streams.contains(name))
        return;

    m_impl->streams[name].stream.str(std::string());
}

std::string log::get_stream_content(const std::string& name) {
    LIBUTIL_ASSERT(m_impl, "Log not initialized.");

    std::lock_guard guard(m_mutex);

    if (!m_impl->streams.contains(name))
        return "";

    return m_impl->streams[name].stream.str();
}

log::settings* log::get_settings() {
    LIBUTIL_ASSERT(m_impl,     "Log not initialized.");
    LIBUTIL_ASSERT(!m_running, "Log not paused.");

    return &m_impl->settings;
}

///////////////////////////////////////////////////////////////////////////////
// LOG IMPL

void log::impl::log_to_streams(const std::string& message) {
    for (auto& [name, map_stream] : streams) {
        if (!map_stream.enabled || !map_stream.stream)
            continue;

        map_stream.stream << message;
    }

    if (!m_impl->settings.log_to_console)
        return;

#ifdef DV_OS_WINDOWS
    auto handle = GetStdHandle(STD_OUTPUT_HANDLE);
    
    if (handle) {
        WriteConsole(handle, message.data(), (DWORD)message.size(), NULL, NULL);
    }
#else
    std::cout << message;
#endif  
}

void log::impl::log_to_file(const std::string& message) const {
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
