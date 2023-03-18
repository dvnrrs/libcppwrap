cmake_minimum_required(VERSION 3.15)

project(
	cppwrap
	VERSION		1.3.4
	DESCRIPTION	"A collection of C++ wrappers for native APIs")

option(ENABLE_LINUX		"Build wrappers for non-POSIX Linux functions"	ON)
option(ENABLE_POSIX		"Build wrappers for POSIX functions"			ON)
option(ENABLE_SOCKETS	"Build wrappers for BSD sockets functions"		ON)
option(ENABLE_WX_IPV6	"Build the IPv6 extensions (requires sockets)"	ON)
option(ENABLE_WX_SLURP	"Build the slurp and spew extensions"			ON)

set(SOURCES
	$<$<STREQUAL:$<UPPER_CASE:${ENABLE_LINUX}>,ON>:src/w/linux.cpp>
	$<$<STREQUAL:$<UPPER_CASE:${ENABLE_POSIX}>,ON>:src/w/posix.cpp>
	$<$<STREQUAL:$<UPPER_CASE:${ENABLE_SOCKETS}>,ON>:src/w/sockets.cpp>
	$<$<STREQUAL:$<UPPER_CASE:${ENABLE_WX_IPV6}>,ON>:src/wx/ipv6.cpp>
	$<$<STREQUAL:$<UPPER_CASE:${ENABLE_WX_SLURP}>,ON>:src/wx/slurp.cpp>
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD			17
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
