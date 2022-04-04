cmake_minimum_required(VERSION 3.2)

project(KaedeManager)

set(BASE "Source/")

if (NOT WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20")
endif()

add_definitions(-DUNICODE -D_UNICODE)

set(SRC_FILES "${BASE}/Core/Reader.cpp" "${BASE}/Collection.cpp" "${BASE}/Core/Writer.cpp")

macro(check_variable_definition var)
    if (NOT ${var})
        message(FATAL_ERROR "${var} Couldn't be found. Did you define it in your project?")
    endif()
endmacro()

check_variable_definition("SPDLOG_INCLUDE_DIR")
check_variable_definition("SPDLOG_SOURCE_DIR")

include_directories(${SPDLOG_INCLUDE_DIR})

add_library(${PROJECT_NAME} ${SRC_FILES} ${SPDLOG_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} spdlog)