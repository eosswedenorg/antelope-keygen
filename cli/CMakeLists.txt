cmake_minimum_required(VERSION 3.15)

# --------------------------------
#  Project Info
# --------------------------------

project(antelope-keygen
	VERSION ${CMAKE_PROJECT_VERSION}
	LANGUAGES CXX)

# Options
option(FORCE_ANSI "Force ANSI console colors even on windows" OFF)

# --------------------------------
#  Program
# --------------------------------

set (PROGRAM_EXE ${PROJECT_NAME})

set (PROGRAM_SOURCE
	src/isatty.cpp
	src/cli_key_search_result.cpp
	src/console.cpp
	src/benchmark.cpp
	src/main.cpp
)

if (WIN32 AND NOT FORCE_ANSI)
	set (PROGRAM_SOURCE ${PROGRAM_SOURCE} src/console_win32.cpp)
else()
	# *nix should have ansi support.
	set (PROGRAM_SOURCE ${PROGRAM_SOURCE} src/console_ansi.cpp)
endif()

add_executable( ${PROGRAM_EXE} ${PROGRAM_SOURCE} )

# Include CLI11
include ( ${CMAKE_SOURCE_DIR}/lib/CLI11/CMakeLists.txt )

target_include_directories( ${PROGRAM_EXE} PRIVATE ${LIBCLI11_INCLUDE} )
target_link_libraries( ${PROGRAM_EXE} PUBLIC common )

# --------------------------------
#  Install
# --------------------------------

install(TARGETS ${PROGRAM_EXE}
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		COMPONENT cli)

install(FILES ${PROJECT_LICENSE_FILE}
		DESTINATION ${CMAKE_INSTALL_SHAREDIR}
		COMPONENT cli
		RENAME LICENSE.cli)

install (FILES ${LIBCLI11_LICENSE}
		DESTINATION ${CMAKE_INSTALL_SHAREDIR}
		COMPONENT cli
		RENAME LICENSE.libcli11)


# Documentation

configure_file( docs/README.md.in ${PROJECT_BINARY_DIR}/README.cli.md @ONLY )
install(FILES ${PROJECT_BINARY_DIR}/README.cli.md
		DESTINATION ${CMAKE_INSTALL_SHAREDIR}
		COMPONENT cli)

if (UNIX)
	configure_file( docs/antelope-keygen.1.in ${PROJECT_BINARY_DIR}/man1/antelope-keygen.1 )
	configure_file( docs/antelope-keygen-search.1.in ${PROJECT_BINARY_DIR}/man1/antelope-keygen-search.1 )
	configure_file( docs/antelope-keygen-benchmark.1.in ${PROJECT_BINARY_DIR}/man1/antelope-keygen-benchmark.1 )

	install(DIRECTORY ${PROJECT_BINARY_DIR}/man1
			DESTINATION ${CMAKE_INSTALL_MANDIR}
			COMPONENT cli)
endif (UNIX)

# --------------------------------
#  Package
# --------------------------------

configure_file(cmake/CPackComponentConfig.cmake.in
	${PROJECT_BINARY_DIR}/CPackProperties.cmake)
