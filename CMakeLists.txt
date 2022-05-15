cmake_minimum_required(VERSION 3.2)

project(KaedeManager)

set(BASE "Source/")
set(CMAKE_CXX_STANDARD 20)

add_definitions(-DUNICODE -D_UNICODE)

macro(add_subdirectory_safe var)
    if (NOT TARGET ${var})
        add_subdirectory("External/${var}")
    else()
        message(STATUS "${var} is already added. nothing changed.")
    endif()
endmacro()

add_subdirectory_safe("fmt")

set(SOURCE_FILES "${BASE}/Core/Reader.cpp" "${BASE}/Collection.cpp" "${BASE}/Core/Writer.cpp")

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC "External/fmt/include")
target_link_libraries(${PROJECT_NAME} fmt)