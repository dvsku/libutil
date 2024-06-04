#include <libutil.hpp>
#include <gtest/gtest.h>

using namespace dvsku;

TEST(string, format) {
    std::string valid = "Hello, dvsku. January has 31 days.";
    std::string fmt = "Hello, {}. January has {} days.";

    std::string formatted = DV_FORMAT(fmt, "dvsku", 31);

    EXPECT_TRUE(valid == formatted);
}

TEST(string, split) {
    std::string str   = "I|am|dvsku";
    auto        split = dv_util_string::split(str, '|');

    EXPECT_TRUE(split.size() == 3);
    EXPECT_TRUE(split[0] == "I");
    EXPECT_TRUE(split[1] == "am");
    EXPECT_TRUE(split[2] == "dvsku");
}

TEST(string, trim) {
    std::string str     = "|I am | dvsku||";
    std::string trimmed = dv_util_string::trim(str, "|");

    EXPECT_TRUE(trimmed == "I am | dvsku");
}

TEST(string, replace) {
    std::string str      = "Hello.";
    std::string replaced = dv_util_string::replace(str, ".", ", dvsku.");

    EXPECT_TRUE(replaced == "Hello, dvsku.");
}

TEST(string, contains) {
    std::string str = "abcd";

    EXPECT_TRUE(dv_util_string::contains(str, "ab"));
    EXPECT_FALSE(dv_util_string::contains(str, "e"));
}