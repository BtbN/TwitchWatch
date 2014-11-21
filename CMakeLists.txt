cmake_minimum_required(VERSION 2.8.12)

project(TwitchWatch)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
set(CMAKE_AUTOMOC TRUE)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

if(${CMAKE_C_COMPILER_ID} MATCHES "Clang" OR ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
	set(CMAKE_COMPILER_IS_CLANG TRUE)
endif()

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG)
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic ${CMAKE_CXX_FLAGS} -std=gnu++11 -fno-strict-aliasing")
	set(CMAKE_C_FLAGS "-Wall -Wextra -pedantic ${CMAKE_C_FLAGS} -std=gnu99 -fno-strict-aliasing")
elseif(MSVC)
	if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4127 /wd4201")
endif()

add_subdirectory(qtspeech)
add_subdirectory(3rdparty/dist)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets Network)

set(TwitchWatch_SOURCES
	main.cpp
	mainwin.cpp
	mainwin.h
	twitchirc.cpp
	twitchirc.h
	srlirc.cpp
	srlirc.h
	streamwatch.cpp
	streamwatch.h)

add_executable(TwitchWatch WIN32
	${TwitchWatch_SOURCES})
target_link_libraries(TwitchWatch
	Qt5::Widgets
	Qt5::Network
	QtSpeech
	communi)
