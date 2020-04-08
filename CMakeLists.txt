cmake_minimum_required(VERSION 3.15)

# --------------------------------
#  Project Info
# --------------------------------

project(eosio-keygen
	VERSION 1.0.4
	DESCRIPTION "Keygenerator for EOSIO"
	HOMEPAGE_URL "https://github.com/eosswedenorg/eosio-keygen" )

set( PROJECT_MAINTAINER "Henrik Hautakoski <henrik@eossweden.org>")

# --------------------------------
#  CMake settings
# --------------------------------

# Append modules dir
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Default to release build if none is set.
if (NOT CMAKE_BUILD_TYPE)
	set( CMAKE_BUILD_TYPE Release )
endif()

# Install path
include(GNUInstallDirs)

set( CMAKE_INSTALL_SHAREDIR ${CMAKE_INSTALL_DATADIR}/${CMAKE_PROJECT_NAME} )

if (WIN32)
	# "Flat" install on windows.
	set( CMAKE_INSTALL_BINDIR "." )
	set( CMAKE_INSTALL_DATADIR "." )
	set( CMAKE_INSTALL_SHAREDIR "." )
	set( CMAKE_INSTALL_MANDIR "." )
endif (WIN32)

# --------------------------------
#  Compiler settings
# --------------------------------

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

# c++ flags
set( CMAKE_CXX_FLAGS_DEBUG "-O0 -g" )
set( CMAKE_CXX_FLAGS_RELEASE "-O3" )
set( CMAKE_CXX_FLAGS_MINSIZEREL "-Os" )

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wconversion -Wno-sign-conversion -Wextra" )
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	add_definitions( "-D_CRT_SECURE_NO_WARNINGS=1" )
endif()

# Project config file
configure_file(config.h.in "${PROJECT_BINARY_DIR}/config.h" @ONLY)
include_directories(${PROJECT_BINARY_DIR})

# --------------------------------
#  Subdirectories
# --------------------------------

add_subdirectory( common )
add_subdirectory( cli )
add_subdirectory( gui )

# --------------------------------
#  Install
# --------------------------------

# Readme and license
install(FILES README.md LICENSE
		DESTINATION ${CMAKE_INSTALL_SHAREDIR})

# --------------------------------
#  Package
# --------------------------------

if (CPACK_GENERATOR MATCHES "^[Nn][Ss][Ii][Ss]$")
	set( CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_LIST_DIR}/LICENSE )
endif()

set( CPACK_DEBIAN_PACKAGE_PRIORITY "optional" )
set( CPACK_DEBIAN_PACKAGE_SECTION "misc" )
set( CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON )

include( cpack_custom )
