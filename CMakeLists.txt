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

option(COMPONENT_CLI "Build CLI Component" ON)
option(COMPONENT_GUI "Build GUI Component (Qt5)" OFF)

if (NOT COMPONENT_CLI AND NOT COMPONENT_GUI)
	message(FATAL_ERROR "Atleast one of BUILD_COMPONENT_GUI,BUILD_COMPONENT_CLI must be set to ON")
endif()

# --------------------------------
#  CMake settings
# --------------------------------

# Append modules dir
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Default to debug build if none is set.
if (NOT CMAKE_BUILD_TYPE)
	set( CMAKE_BUILD_TYPE Debug )
endif()

# Install path
include(GNUInstallDirs)

set( CMAKE_INSTALL_SHAREDIR ${CMAKE_INSTALL_DATADIR}/${CMAKE_PROJECT_NAME} )

if (WIN32)
	# "Flat" install on windows.
	set( CMAKE_INSTALL_BINDIR "bin" )
	set( CMAKE_INSTALL_DATADIR "." )
	set( CMAKE_INSTALL_SHAREDIR "." )
	set( CMAKE_INSTALL_MANDIR "." )
endif (WIN32)

# Path to a directory outside of CMAKE_BUILD_DIR
# Should be used to cache large downloaded data that won't be deleted on clean builds.
set( DOWNLOAD_CACHE_DIR ${CMAKE_CURRENT_LIST_DIR}/.cache )

set( components )
if (COMPONENT_CLI)
	list(APPEND components cli )
endif()

if (COMPONENT_GUI)
	list(APPEND components gui )
endif()

# --------------------------------
#  Compiler settings
# --------------------------------

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

# c++ flags
add_compile_options(
	"$<$<CXX_COMPILER_ID:GNU>:-Werror;-Wall;-Wconversion;-Wno-sign-conversion;-Wextra>"
	"$<$<CXX_COMPILER_ID:MSVC>:/W3;-D_CRT_SECURE_NO_WARNINGS=1>"

	# Debug
	"$<$<CONFIG:Debug>:$<$<CXX_COMPILER_ID:GNU>:-O0;-g>>"
	"$<$<CONFIG:Debug>:$<$<CXX_COMPILER_ID:MSVC>:/Od;/Zi>>"

	# Release
	"$<$<CONFIG:Release>:$<$<CXX_COMPILER_ID:GNU>:-O3>>"
	"$<$<CONFIG:Release>:$<$<CXX_COMPILER_ID:MSVC>:/O2>>"

	# MinSizeRel
	"$<$<CONFIG:MinSizeRel>:$<$<CXX_COMPILER_ID:GNU>:-Os>>"
	"$<$<CONFIG:MinSizeRel>:$<$<CXX_COMPILER_ID:MSVC>:/O1>>"
)

# Project config file
configure_file(config.hpp.in "${PROJECT_BINARY_DIR}/config.hpp" @ONLY)
include_directories(${PROJECT_BINARY_DIR})

# Bundle eosio-extras on windows.
if (WIN32)
	include(extras)
	list(APPEND components extras )
endif()

# --------------------------------
#  Package
# --------------------------------

include(CPackConfig)

set( CPACK_COMPONENTS_ALL ${components} )

if (CPACK_GENERATOR MATCHES "^[Nn][Ss][Ii][Ss]$")
	set( CPACK_RESOURCE_FILE_LICENSE ${PROJECT_LICENSE_FILE} )
endif()

set (CPACK_PROPERTIES_FILE ${PROJECT_BINARY_DIR}/CPackComponentsInclude.cmake)
configure_file(cmake/CPackProperties.cmake.in ${CPACK_PROPERTIES_FILE} @ONLY)

# --------------------------------
#  Components
# --------------------------------

add_subdirectory( common )

foreach(comp ${components})
	if ( EXISTS ${CMAKE_CURRENT_LIST_DIR}/${comp} )
		add_subdirectory( ${comp} )
	endif()
endforeach()

# --------------------------------
#  CPack
# --------------------------------
include (CPack)
