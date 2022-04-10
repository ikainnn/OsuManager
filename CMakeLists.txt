cmake_minimum_required(VERSION 3.2)

project(KaedeManager)

set(BASE "Source/")
set(CMAKE_CXX_STANDARD 20)

add_definitions(-DUNICODE -D_UNICODE)

set(SOURCE_FILES "${BASE}/Core/Reader.cpp" "${BASE}/Collection.cpp" "${BASE}/Core/Writer.cpp")

macro(add_subdirectory_safe var)
    if (NOT TARGET ${var})
        add_subdirectory("External/${var}")
    else()
        message(STATUS "${var} is already added. nothing changed.")
    endif()
endmacro()

add_subdirectory_safe("spdlog")

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${SPDLOG_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} spdlog)