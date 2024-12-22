###############################################################################
## googletest

IF(LIBUTIL_TOP_LEVEL AND LIBUTIL_TEST)
    SET(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )

    FetchContent_MakeAvailable(googletest)

    SET(GTEST_INCLUDES "${googletest_SOURCE_DIR}/googletest/include" CACHE INTERNAL "")
ENDIF()
