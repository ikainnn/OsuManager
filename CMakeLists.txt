cmake_minimum_required(VERSION 3.2)

project(KaedeManager)

set(BASE "Source/")
set(CMAKE_CXX_STANDARD 23)

add_definitions(-DUNICODE -D_UNICODE)

set(SOURCE_FILES "${BASE}/Core/Reader.cpp" "${BASE}/Collection.cpp" "${BASE}/Core/Writer.cpp")

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC)

target_link_libraries(${PROJECT_NAME})