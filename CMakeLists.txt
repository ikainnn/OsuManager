cmake_minimum_required(VERSION 3.2)

project(KaedeManager)

set(BASE "Source/KaedeApi")

if (NOT WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20")
endif()

add_definitions(-DUNICODE -D_UNICODE)

set(SRC_FILES "${BASE}/Core/Reader.cpp" 
			  "${BASE}/Collection.cpp" 
			  "${BASE}/Core/Writer.cpp")

add_library(${PROJECT_NAME} ${SRC_FILES})