#include <libutil.hpp>
#include <gtest/gtest.h>
#include <algorithm>

using namespace dvsku;

static size_t get_line_count(const std::string& str) {
    std::vector<std::string> lines = dv_util_string::split(str, '\n');

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
    dv_util_log::settings settings;
    settings.level                        = dv_util_log::level::debug;
    settings.log_to_file                  = false;
    settings.log_message_timestamp_format = "";
    
    dv_util_log::init(settings);
    dv_util_log::create_source("sstream", &ss);
    
    {
        dv_util_log::log_message(dv_util_log::level::debug, "testing", "I am {}.", "dvsku");
        std::string str = ss.str();

        EXPECT_TRUE(str != "");
        EXPECT_TRUE(str == "[DEBG] [testing] I am dvsku.\n");
    }

    ss.str(std::string());

    {
        dv_util_log::log_message(dv_util_log::level::debug, "", "I am {}.", "dvsku");
        std::string str = ss.str();

        EXPECT_TRUE(str != "");
        EXPECT_TRUE(str == "[DEBG] I am dvsku.\n");
    }
}

TEST(log, levels) {
    std::stringstream ss;
    dv_util_log::settings settings;
    settings.level       = dv_util_log::level::none;
    settings.log_to_file = false;

    dv_util_log::init(settings);
    dv_util_log::create_source("sstream", &ss);

    {
        dv_util_log::log_message(dv_util_log::level::informational, "", "");
        dv_util_log::log_message(dv_util_log::level::warning, "", "");
        dv_util_log::log_message(dv_util_log::level::error, "", "");
        dv_util_log::log_message(dv_util_log::level::verbose, "", "");
        dv_util_log::log_message(dv_util_log::level::debug, "", "");

        std::string str = ss.str();
        EXPECT_TRUE(get_line_count(str) == 0);
    }

    ss.str(std::string());
    dv_util_log::get_settings()->level = dv_util_log::level::informational;

    {
        dv_util_log::log_message(dv_util_log::level::informational, "", "");
        dv_util_log::log_message(dv_util_log::level::warning, "", "");
        dv_util_log::log_message(dv_util_log::level::error, "", "");
        dv_util_log::log_message(dv_util_log::level::verbose, "", "");
        dv_util_log::log_message(dv_util_log::level::debug, "", "");

        std::string str = ss.str();
        EXPECT_TRUE(get_line_count(str) == 1);
    }

    ss.str(std::string());
    dv_util_log::get_settings()->level = dv_util_log::level::warning;

    {
        dv_util_log::log_message(dv_util_log::level::informational, "", "");
        dv_util_log::log_message(dv_util_log::level::warning, "", "");
        dv_util_log::log_message(dv_util_log::level::error, "", "");
        dv_util_log::log_message(dv_util_log::level::verbose, "", "");
        dv_util_log::log_message(dv_util_log::level::debug, "", "");

        std::string str = ss.str();
        EXPECT_TRUE(get_line_count(str) == 2);
    }

    ss.str(std::string());
    dv_util_log::get_settings()->level = dv_util_log::level::error;

    {
        dv_util_log::log_message(dv_util_log::level::informational, "", "");
        dv_util_log::log_message(dv_util_log::level::warning, "", "");
        dv_util_log::log_message(dv_util_log::level::error, "", "");
        dv_util_log::log_message(dv_util_log::level::verbose, "", "");
        dv_util_log::log_message(dv_util_log::level::debug, "", "");

        std::string str = ss.str();
        EXPECT_TRUE(get_line_count(str) == 3);
    }

    ss.str(std::string());
    dv_util_log::get_settings()->level = dv_util_log::level::verbose;

    {
        dv_util_log::log_message(dv_util_log::level::informational, "", "");
        dv_util_log::log_message(dv_util_log::level::warning, "", "");
        dv_util_log::log_message(dv_util_log::level::error, "", "");
        dv_util_log::log_message(dv_util_log::level::verbose, "", "");
        dv_util_log::log_message(dv_util_log::level::debug, "", "");

        std::string str = ss.str();
        EXPECT_TRUE(get_line_count(str) == 4);
    }

    ss.str(std::string());
    dv_util_log::get_settings()->level = dv_util_log::level::debug;

    {
        dv_util_log::log_message(dv_util_log::level::informational, "", "");
        dv_util_log::log_message(dv_util_log::level::warning, "", "");
        dv_util_log::log_message(dv_util_log::level::error, "", "");
        dv_util_log::log_message(dv_util_log::level::verbose, "", "");
        dv_util_log::log_message(dv_util_log::level::debug, "", "");

        std::string str = ss.str();
        EXPECT_TRUE(get_line_count(str) == 5);
    }
}

TEST(log, log_file_create) {
    dv_util_log::settings settings;
    settings.level       = dv_util_log::level::informational;
    settings.log_to_file = true;

    dv_util_log::init(settings);   
    dv_util_log::log_message(dv_util_log::level::informational, "", "");

    auto localtime = dv_util_datetime::localtime_now();

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