cmake_minimum_required(VERSION 3.15)

# Project name and version
project(eosio-keygen VERSION 1.0.2)

set( PROJECT_DESCRIPTION "Keygenerator for EOSIO" )
set( PROJECT_HOMEPAGE_URL "https://github.com/eosswedenorg/eosio-keygen" )
set( PROJECT_MAINTAINER "Henrik Hautakoski <henrik@eossweden.org>")

# Options
option(USE_THREADS "Compile with support for threads (if available)." ON)
option(FORCE_ANSI "Force ANSI console colors even on windows" OFF)
option(EXTRAS "Include non essential (extra) files in the build. (only for *nix)" OFF)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

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
#  Additional project dirs
# --------------------------------

add_subdirectory( cli )
add_subdirectory( docs )

if (UNIX AND EXTRAS)
	add_subdirectory( extras )
endif()

# --------------------------------
#  Additional install files
# --------------------------------

install(FILES README.md LICENSE LICENSE.bitcoin
		DESTINATION ${CMAKE_INSTALL_SHAREDIR})
