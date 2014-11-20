project(QtSpeech)

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
set(CMAKE_AUTOMOC TRUE)

find_package(Qt5Core REQUIRED)

set(QtSpeech_SOURCES
	QtSpeech
	QtSpeech.h)

if(WIN32)
	set(QtSpeech_PLATFORM_SOURCES
		QtSpeech_win.cpp)
elseif(APPLE)
	set(QtSpeech_PLATFORM_SOURCES
		QtSpeech_mac.cpp)
else()
	set(QtSpeech_PLATFORM_SOURCES
		QtSpeech_unx.cpp)
endif()

add_library(QtSpeech STATIC
	${QtSpeech_SOURCES}
	${QtSpeech_PLATFORM_SOURCES})
target_link_libraries(QtSpeech
	Qt5::Core)
target_compile_definitions(QtSpeech
	PUBLIC
		QTSPEECH_STATIC)
target_include_directories(QtSpeech
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR})
