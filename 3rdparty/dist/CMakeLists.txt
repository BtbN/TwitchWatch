
if(UNIX AND NOT APPLE)
	set(_libs
		"${CMAKE_CURRENT_SOURCE_DIR}/linux_x64/lib/libIrcCore.a"
		"${CMAKE_CURRENT_SOURCE_DIR}/linux_x64/lib/libIrcModel.a"
		"${CMAKE_CURRENT_SOURCE_DIR}/linux_x64/lib/libIrcUtil.a")
	set(_inc
		"${CMAKE_CURRENT_SOURCE_DIR}/linux_x64/include"
		"${CMAKE_CURRENT_SOURCE_DIR}/linux_x64/include/IrcCore"
		"${CMAKE_CURRENT_SOURCE_DIR}/linux_x64/include/IrcModel"
		"${CMAKE_CURRENT_SOURCE_DIR}/linux_x64/include/IrcUtil")
else()
	message(FATAL_ERROR "No prebuilt communi available")
endif()

add_library(communi INTERFACE IMPORTED GLOBAL)
set_property(TARGET communi PROPERTY INTERFACE_LINK_LIBRARIES "${_libs}")
set_property(TARGET communi PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${_inc}")
set_property(TARGET communi PROPERTY INTERFACE_COMPILE_DEFINITIONS "IRC_STATIC")
