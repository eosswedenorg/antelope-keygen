cmake_minimum_required(VERSION 3.15)

# Project name and version
project(eosio-keygen
	VERSION 1.0.3
	DESCRIPTION "Keygenerator for EOSIO"
	HOMEPAGE_URL "https://github.com/eosswedenorg/eosio-keygen" )

set( PROJECT_MAINTAINER "Henrik Hautakoski <henrik@eossweden.org>")

# Options
option(USE_THREADS "Compile with support for threads (if available)." ON)
option(FORCE_ANSI "Force ANSI console colors even on windows" OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

include(libeoskeygen)

# Use installpath from GNUInstallDirs as default.
include(GNUInstallDirs)

set( CMAKE_INSTALL_SHAREDIR ${CMAKE_INSTALL_DATADIR}/${CMAKE_PROJECT_NAME} )

if (WIN32)
	# "Flat" install on windows.
	set( CMAKE_INSTALL_BINDIR "." )
	set( CMAKE_INSTALL_DATADIR "." )
	set( CMAKE_INSTALL_SHAREDIR "." )
	set( CMAKE_INSTALL_MANDIR "." )
endif (WIN32)

# Configure the compiler options
set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wconversion -Wno-sign-conversion -Wextra" )
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	add_definitions( "-D_CRT_SECURE_NO_WARNINGS=1" )
endif()

# --------------------------------
#  Program
# --------------------------------

set (PROGRAM_EXE ${CMAKE_PROJECT_NAME})

set (PROGRAM_SOURCE
	src/core/isatty.cpp
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

# Project config file
configure_file(src/config.h.in "${PROJECT_BINARY_DIR}/config.h" @ONLY)
include_directories(${PROJECT_BINARY_DIR})

# Include OpenSSL headers
#include_directories( ${OPENSSL_INCLUDE_DIR} )

add_executable( ${PROGRAM_EXE} ${PROGRAM_SOURCE} )

target_link_libraries( ${PROGRAM_EXE} PUBLIC eoskeygen )

# --------------------------------
#  Install
# --------------------------------

install(TARGETS ${PROGRAM_EXE} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# Readme and license
install(FILES README.md LICENSE LICENSE.bitcoin
		DESTINATION ${CMAKE_INSTALL_SHAREDIR})

# Documentation

if (UNIX)
	configure_file( docs/eosio-keygen.1.in ${PROJECT_BINARY_DIR}/man1/eosio-keygen.1 )

	install(DIRECTORY ${PROJECT_BINARY_DIR}/man1
			DESTINATION ${CMAKE_INSTALL_MANDIR})
endif (UNIX)

# --------------------------------
#  Package
# --------------------------------

set( CPACK_DEBIAN_PACKAGE_PRIORITY "optional" )
set( CPACK_DEBIAN_PACKAGE_SECTION "misc" )
set( CPACK_DEBIAN_PACKAGE_DEPENDS "libssl1.1, libc6" )

include( cpack_custom )
