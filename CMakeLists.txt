cmake_minimum_required(VERSION 3.2)

project(KaedeManager)
set(CMAKE_CXX_STANDARD 20)

add_definitions(-DUNICODE -D_UNICODE)

set(SOURCE_FILES "Source/Collection.cpp" "Source/File/Reader.cpp" "Source/File/Writer.cpp")

add_library(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE "External/fmt/include")
target_link_libraries(${PROJECT_NAME} PRIVATE fmt)