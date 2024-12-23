###############################################################################
## eventpp

FetchContent_Declare(
    eventpp
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/eventpp"
)

FetchContent_MakeAvailable(eventpp)

SET(EVENTPP_INCLUDES "${eventpp_SOURCE_DIR}/eventpp/include" CACHE INTERNAL "")
