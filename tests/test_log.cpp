#include <libutil.hpp>
#include <gtest/gtest.h>
#include <algorithm>

using namespace libutil;

static size_t get_line_count(const std::string& str) {
    std::vector<std::string> lines = string::split(str, '\n');

    lines.erase(std::remove_if(lines.begin(), lines.end(),
        [](const std::string& str) {
            return str.empty();
        }),
        lines.end()
    );

    return lines.size();
}

TEST(log, message_to_sstream) {
    log::settings settings;
    settings.level                        = log::level::debug;
    settings.log_to_console               = false;
    settings.log_to_file                  = false;
    settings.log_message_timestamp_format = "";
    
    log::init(settings);
    log::create_stream("sstream");
    
    {
        log::log_message(log::level::debug, "testing", "I am {}.", "dvsku");
        std::string str = log::get_stream_content("sstream");

        EXPECT_TRUE(str != "");
        EXPECT_TRUE(str == "[DEBG] [testing] I am dvsku.\n");
    }

    log::clear_stream("sstream");

    {
        log::log_message(log::level::debug, "", "I am {}.", "dvsku");
        std::string str = log::get_stream_content("sstream");

        EXPECT_TRUE(str != "");
        EXPECT_TRUE(str == "[DEBG] I am dvsku.\n");
    }
}

TEST(log, levels) {
    log::settings settings;
    settings.level          = log::level::none;
    settings.log_to_console = false;
    settings.log_to_file    = false;

    log::init(settings);
    log::create_stream("sstream");

    {
        log::log_message(log::level::informational, "", "");
        log::log_message(log::level::warning, "", "");
        log::log_message(log::level::error, "", "");
        log::log_message(log::level::verbose, "", "");
        log::log_message(log::level::debug, "", "");

        std::string str = log::get_stream_content("sstream");
        EXPECT_TRUE(get_line_count(str) == 0);
    }

    log::clear_stream("sstream");
    log::pause();
    log::get_settings()->level = log::level::informational;
    log::resume();

    {
        log::log_message(log::level::informational, "", "");
        log::log_message(log::level::warning, "", "");
        log::log_message(log::level::error, "", "");
        log::log_message(log::level::verbose, "", "");
        log::log_message(log::level::debug, "", "");

        std::string str = log::get_stream_content("sstream");
        EXPECT_TRUE(get_line_count(str) == 1);
    }

    log::clear_stream("sstream");
    log::pause();
    log::get_settings()->level = log::level::warning;
    log::resume();

    {
        log::log_message(log::level::informational, "", "");
        log::log_message(log::level::warning, "", "");
        log::log_message(log::level::error, "", "");
        log::log_message(log::level::verbose, "", "");
        log::log_message(log::level::debug, "", "");

        std::string str = log::get_stream_content("sstream");
        EXPECT_TRUE(get_line_count(str) == 2);
    }

    log::clear_stream("sstream");
    log::pause();
    log::get_settings()->level = log::level::error;
    log::resume();

    {
        log::log_message(log::level::informational, "", "");
        log::log_message(log::level::warning, "", "");
        log::log_message(log::level::error, "", "");
        log::log_message(log::level::verbose, "", "");
        log::log_message(log::level::debug, "", "");

        std::string str = log::get_stream_content("sstream");
        EXPECT_TRUE(get_line_count(str) == 3);
    }

    log::clear_stream("sstream");
    log::pause();
    log::get_settings()->level = log::level::verbose;
    log::resume();

    {
        log::log_message(log::level::informational, "", "");
        log::log_message(log::level::warning, "", "");
        log::log_message(log::level::error, "", "");
        log::log_message(log::level::verbose, "", "");
        log::log_message(log::level::debug, "", "");

        std::string str = log::get_stream_content("sstream");
        EXPECT_TRUE(get_line_count(str) == 4);
    }

    log::clear_stream("sstream");
    log::pause();
    log::get_settings()->level = log::level::debug;
    log::resume();

    {
        log::log_message(log::level::informational, "", "");
        log::log_message(log::level::warning, "", "");
        log::log_message(log::level::error, "", "");
        log::log_message(log::level::verbose, "", "");
        log::log_message(log::level::debug, "", "");

        std::string str = log::get_stream_content("sstream");
        EXPECT_TRUE(get_line_count(str) == 5);
    }
}

TEST(log, log_file_create) {
    log::settings settings;
    settings.level          = log::level::informational;
    settings.log_to_console = false;
    settings.log_to_file    = true;

    log::init(settings);   
    log::log_message(log::level::informational, "", "");

    auto localtime = datetime::localtime_now();

    std::string filename_format = settings.log_file_name;
    filename_format.append("_");
    filename_format.append(settings.log_file_timestamp_format);
    filename_format.append(".txt");

    std::string filename = DV_FORMAT(filename_format, localtime);

    bool file_exists = std::filesystem::exists(filename);
    EXPECT_TRUE(file_exists);

    if (file_exists)
        std::filesystem::remove(filename);
}