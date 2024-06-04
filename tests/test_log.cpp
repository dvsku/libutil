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
    util_log::settings settings;
    settings.level                        = util_log::level::debug;
    settings.log_to_file                  = false;
    settings.log_message_timestamp_format = "";
    
    util_log::init(settings);
    util_log::create_source("sstream", &ss);
    
    {
        util_log::log_message(util_log::level::debug, "testing", "I am {}.", "dvsku");
        std::string str = ss.str();

        EXPECT_TRUE(str != "");
        EXPECT_TRUE(str == "[DEBG] [testing] I am dvsku.\n");
    }

    ss.str(std::string());

    {
        util_log::log_message(util_log::level::debug, "", "I am {}.", "dvsku");
        std::string str = ss.str();

        EXPECT_TRUE(str != "");
        EXPECT_TRUE(str == "[DEBG] I am dvsku.\n");
    }
}

TEST(log, levels) {
    std::stringstream ss;
    util_log::settings settings;
    settings.level       = util_log::level::none;
    settings.log_to_file = false;

    util_log::init(settings);
    util_log::create_source("sstream", &ss);

    {
        util_log::log_message(util_log::level::informational, "", "");
        util_log::log_message(util_log::level::warning, "", "");
        util_log::log_message(util_log::level::error, "", "");
        util_log::log_message(util_log::level::verbose, "", "");
        util_log::log_message(util_log::level::debug, "", "");

        std::string str = ss.str();
        EXPECT_TRUE(get_line_count(str) == 0);
    }

    ss.str(std::string());
    util_log::get_settings()->level = util_log::level::informational;

    {
        util_log::log_message(util_log::level::informational, "", "");
        util_log::log_message(util_log::level::warning, "", "");
        util_log::log_message(util_log::level::error, "", "");
        util_log::log_message(util_log::level::verbose, "", "");
        util_log::log_message(util_log::level::debug, "", "");

        std::string str = ss.str();
        EXPECT_TRUE(get_line_count(str) == 1);
    }

    ss.str(std::string());
    util_log::get_settings()->level = util_log::level::warning;

    {
        util_log::log_message(util_log::level::informational, "", "");
        util_log::log_message(util_log::level::warning, "", "");
        util_log::log_message(util_log::level::error, "", "");
        util_log::log_message(util_log::level::verbose, "", "");
        util_log::log_message(util_log::level::debug, "", "");

        std::string str = ss.str();
        EXPECT_TRUE(get_line_count(str) == 2);
    }

    ss.str(std::string());
    util_log::get_settings()->level = util_log::level::error;

    {
        util_log::log_message(util_log::level::informational, "", "");
        util_log::log_message(util_log::level::warning, "", "");
        util_log::log_message(util_log::level::error, "", "");
        util_log::log_message(util_log::level::verbose, "", "");
        util_log::log_message(util_log::level::debug, "", "");

        std::string str = ss.str();
        EXPECT_TRUE(get_line_count(str) == 3);
    }

    ss.str(std::string());
    util_log::get_settings()->level = util_log::level::verbose;

    {
        util_log::log_message(util_log::level::informational, "", "");
        util_log::log_message(util_log::level::warning, "", "");
        util_log::log_message(util_log::level::error, "", "");
        util_log::log_message(util_log::level::verbose, "", "");
        util_log::log_message(util_log::level::debug, "", "");

        std::string str = ss.str();
        EXPECT_TRUE(get_line_count(str) == 4);
    }

    ss.str(std::string());
    util_log::get_settings()->level = util_log::level::debug;

    {
        util_log::log_message(util_log::level::informational, "", "");
        util_log::log_message(util_log::level::warning, "", "");
        util_log::log_message(util_log::level::error, "", "");
        util_log::log_message(util_log::level::verbose, "", "");
        util_log::log_message(util_log::level::debug, "", "");

        std::string str = ss.str();
        EXPECT_TRUE(get_line_count(str) == 5);
    }
}

TEST(log, log_file_create) {
    util_log::settings settings;
    settings.level       = util_log::level::informational;
    settings.log_to_file = true;

    util_log::init(settings);   
    util_log::log_message(util_log::level::informational, "", "");

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