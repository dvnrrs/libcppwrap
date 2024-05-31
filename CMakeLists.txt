cmake_minimum_required(VERSION 3.15)

project(
	cppwrap
	VERSION		1.3.12
	DESCRIPTION	"A collection of C++ wrappers for native APIs")

option(ENABLE_LINUX		"Build wrappers for non-POSIX Linux functions"	ON)
option(ENABLE_POSIX		"Build wrappers for POSIX functions"			ON)
option(ENABLE_SOCKETS	"Build wrappers for BSD sockets functions"		ON)
option(ENABLE_WX_IPV6	"Build the IPv6 extensions (requires sockets)"	ON)
option(ENABLE_WX_SLURP	"Build the slurp and spew extensions"			ON)

if(ENABLE_LINUX)
	list(APPEND SOURCES "w/linux.cpp")
endif()

if(ENABLE_POSIX)
	list(APPEND SOURCES "w/posix.cpp")
endif()

if(ENABLE_SOCKETS)
	list(APPEND SOURCES "w/sockets.cpp")
endif()

if(ENABLE_WX_IPV6)
	list(APPEND SOURCES "wx/ipv6.cpp")
endif()

if(ENABLE_WX_SLURP)
	list(APPEND SOURCES "wx/slurp.cpp")
endif()

list(TRANSFORM SOURCES PREPEND src/)
add_library(${PROJECT_NAME} ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD			20
	CXX_STANDARD_REQUIRED	yes
	CXX_EXTENSIONS			no
	SOVERSION				${CMAKE_PROJECT_VERSION}
)

target_include_directories(${PROJECT_NAME}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>)

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME})
install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX} FILES_MATCHING PATTERN "*.hpp")
install(EXPORT ${PROJECT_NAME} DESTINATION lib/cmake/${PROJECT_NAME})
install(FILES cmake/${PROJECT_NAME}Config.cmake DESTINATION lib/cmake/${PROJECT_NAME})
