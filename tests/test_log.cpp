#include <libutil.hpp>
#include <gtest/gtest.h>
#include <algorithm>

using namespace dvsku;

static size_t get_line_count(const std::string& str) {
    std::vector<std::string> lines = util_string::split(str, '\n');

    lines.erase(std::remove_if(lines.begin(), lines.end(),
        [](const std::string& str) {
            return str.empty();
        }),
        lines.end()
    );

    return lines.size();
}

TEST(log, message_to_sstream) {
    std::stringstream ss;
    log::settings settings;
    settings.level                        = log::level::debug;
    settings.log_to_file                  = false;
    settings.log_message_timestamp_format = "";
    
    log::init(settings);
    log::create_source("sstream", &ss);
    
    {
        log::log_message(log::level::debug, "testing", "I am {}.", "dvsku");
        std::string str = ss.str();

        EXPECT_TRUE(str != "");
        EXPECT_TRUE(str == "[DEBG] [testing] I am dvsku.\n");
    }

    ss.str(std::string());

    {
        log::log_message(log::level::debug, "", "I am {}.", "dvsku");
        std::string str = ss.str();

        EXPECT_TRUE(str != "");
        EXPECT_TRUE(str == "[DEBG] I am dvsku.\n");
    }
}

TEST(log, levels) {
    std::stringstream ss;
    log::settings settings;
    settings.level       = log::level::none;
    settings.log_to_file = false;

    log::init(settings);
    log::create_source("sstream", &ss);

    {
        log::log_message(log::level::informational, "", "");
        log::log_message(log::level::warning, "", "");
        log::log_message(log::level::error, "", "");
        log::log_message(log::level::verbose, "", "");
        log::log_message(log::level::debug, "", "");

        std::string str = ss.str();
        EXPECT_TRUE(get_line_count(str) == 0);
    }

    ss.str(std::string());
    log::get_settings()->level = log::level::informational;

    {
        log::log_message(log::level::informational, "", "");
        log::log_message(log::level::warning, "", "");
        log::log_message(log::level::error, "", "");
        log::log_message(log::level::verbose, "", "");
        log::log_message(log::level::debug, "", "");

        std::string str = ss.str();
        EXPECT_TRUE(get_line_count(str) == 1);
    }

    ss.str(std::string());
    log::get_settings()->level = log::level::warning;

    {
        log::log_message(log::level::informational, "", "");
        log::log_message(log::level::warning, "", "");
        log::log_message(log::level::error, "", "");
        log::log_message(log::level::verbose, "", "");
        log::log_message(log::level::debug, "", "");

        std::string str = ss.str();
        EXPECT_TRUE(get_line_count(str) == 2);
    }

    ss.str(std::string());
    log::get_settings()->level = log::level::error;

    {
        log::log_message(log::level::informational, "", "");
        log::log_message(log::level::warning, "", "");
        log::log_message(log::level::error, "", "");
        log::log_message(log::level::verbose, "", "");
        log::log_message(log::level::debug, "", "");

        std::string str = ss.str();
        EXPECT_TRUE(get_line_count(str) == 3);
    }

    ss.str(std::string());
    log::get_settings()->level = log::level::verbose;

    {
        log::log_message(log::level::informational, "", "");
        log::log_message(log::level::warning, "", "");
        log::log_message(log::level::error, "", "");
        log::log_message(log::level::verbose, "", "");
        log::log_message(log::level::debug, "", "");

        std::string str = ss.str();
        EXPECT_TRUE(get_line_count(str) == 4);
    }

    ss.str(std::string());
    log::get_settings()->level = log::level::debug;

    {
        log::log_message(log::level::informational, "", "");
        log::log_message(log::level::warning, "", "");
        log::log_message(log::level::error, "", "");
        log::log_message(log::level::verbose, "", "");
        log::log_message(log::level::debug, "", "");

        std::string str = ss.str();
        EXPECT_TRUE(get_line_count(str) == 5);
    }
}

TEST(log, log_file_create) {
    log::settings settings;
    settings.level       = log::level::informational;
    settings.log_to_file = true;

    log::init(settings);   
    log::log_message(log::level::informational, "", "");

    auto localtime = util_datetime::localtime_now();

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