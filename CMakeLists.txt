cmake_minimum_required(VERSION 3.15)

# --------------------------------
#  Project Info
# --------------------------------

project(eosio-keygen
	VERSION 1.0.4
	DESCRIPTION "Keygenerator for EOSIO"
	HOMEPAGE_URL "https://github.com/eosswedenorg/eosio-keygen" )

set( PROJECT_MAINTAINER "Henrik Hautakoski <henrik@eossweden.org>")
set( PROJECT_LICENSE_FILE ${CMAKE_CURRENT_LIST_DIR}/LICENSE )

# --------------------------------
#  Options
# --------------------------------

option(BUILD_COMPONENT_CLI "Build CLI Component" ON)
option(BUILD_COMPONENT_GUI "Build GUI Component (Qt5)" OFF)

if (NOT BUILD_COMPONENT_CLI AND NOT BUILD_COMPONENT_GUI)
	message(FATAL_ERROR "Atleast one of BUILD_COMPONENT_GUI,BUILD_COMPONENT_CLI must be set to ON")
endif()

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

# Path to a directory outside of CMAKE_BUILD_DIR
# Should be used to cache large downloaded data that won't be deleted on clean builds.
set( DOWNLOAD_CACHE_DIR ${CMAKE_CURRENT_LIST_DIR}/.cache )

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
configure_file(config.hpp.in "${PROJECT_BINARY_DIR}/config.hpp" @ONLY)
include_directories(${PROJECT_BINARY_DIR})

# --------------------------------
#  Install
# --------------------------------

# Readme and license
install(FILES README.md ${PROJECT_LICENSE_FILE}
		DESTINATION ${CMAKE_INSTALL_SHAREDIR})

# --------------------------------
#  Package
# --------------------------------

include(CPackConfig)

set( CPACK_COMPONENTS_ALL cli gui )

if (CPACK_GENERATOR MATCHES "^[Nn][Ss][Ii][Ss]$")
	set( CPACK_RESOURCE_FILE_LICENSE ${PROJECT_LICENSE_FILE} )
endif()

set (CPACK_PROPERTIES_FILE ${PROJECT_BINARY_DIR}/CPackComponentsInclude.cmake)
configure_file(cmake/CPackProperties.cmake.in ${CPACK_PROPERTIES_FILE} @ONLY)

# --------------------------------
#  Components
# --------------------------------

add_subdirectory( common )

if (BUILD_COMPONENT_CLI)
	add_subdirectory( cli )
endif()

if (BUILD_COMPONENT_GUI)
	add_subdirectory( gui )
endif()

# --------------------------------
#  CPack
# --------------------------------
include (CPack)
