﻿CMAKE_MINIMUM_REQUIRED(VERSION 3.14)

OPTION(LIBUTIL_TEST "Build tests." ON)

PROJECT(libutil C CXX)

GET_DIRECTORY_PROPERTY(PARENT_DIR PARENT_DIRECTORY)
IF(PARENT_DIR STREQUAL "")
    SET(LIBUTIL_TOP_LEVEL TRUE)
ELSE()
    SET(LIBUTIL_TOP_LEVEL FALSE)
ENDIF()

IF(POLICY CMP0135)
    CMAKE_POLICY(SET CMP0135 NEW)
ENDIF()

IF(LIBUTIL_TOP_LEVEL AND LIBUTIL_TEST)
    ENABLE_TESTING()
ENDIF()

SET(LIBUTIL_ROOT      "${PROJECT_SOURCE_DIR}")
SET(LIBUTIL_HEADERS   "${PROJECT_SOURCE_DIR}/include")
SET(LIBUTIL_LIBRARIES "${PROJECT_SOURCE_DIR}/libraries")

IF(LIBUTIL_TOP_LEVEL)
    IF(UNIX AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        SET(CMAKE_CXX_STANDARD 20)
        SET(CMAKE_CXX_FLAGS         "-pthread -Wall")
        SET(CMAKE_CXX_FLAGS_DEBUG   "-O0")
        SET(CMAKE_CXX_FLAGS_RELEASE "-O2")

        SET(OUTPUT_DIR "binaries/linux/${CMAKE_CXX_COMPILER_ID}/${CMAKE_BUILD_TYPE}")
        STRING(TOLOWER ${OUTPUT_DIR} OUTPUT_DIR_LOWER)
    ELSEIF(MSVC)
        SET(CMAKE_CXX_FLAGS         "/std:c++20 /DWIN32 /D_WINDOWS /W3 /EHsc /GR /GL-")
        SET(CMAKE_CXX_FLAGS_DEBUG   "/MTd /Od /Ob0 /RTC1 /Zi")
        SET(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /DNDEBUG")

        SET(OUTPUT_DIR "binaries/windows/${CMAKE_CXX_COMPILER_ID}/${CMAKE_BUILD_TYPE}")
        STRING(TOLOWER ${OUTPUT_DIR} OUTPUT_DIR_LOWER)
    ELSE()
        MESSAGE(FATAL_ERROR "Unsupported platform or compiler.")
    ENDIF()

    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${OUTPUT_DIR_LOWER}")
    SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${OUTPUT_DIR_LOWER}")
    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/${OUTPUT_DIR_LOWER}")
ENDIF()

ADD_SUBDIRECTORY("dependencies")
ADD_SUBDIRECTORY("source")

IF(LIBUTIL_TOP_LEVEL AND LIBUTIL_TEST)
    ADD_SUBDIRECTORY("tests")
ENDIF()
