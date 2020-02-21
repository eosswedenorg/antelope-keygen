cmake_minimum_required(VERSION 3.4)

# Project name and version
project(eosio-keygen VERSION 1.0)

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

# --------------------------------
#  Program
# --------------------------------

set (PROGRAM_EXE ${CMAKE_PROJECT_NAME})

set (PROGRAM_SOURCE
	src/core/file.cpp
	src/core/dictionary.cpp
	src/core/string.cpp
	src/crypto/base58.cpp
	src/crypto/WIF.cpp
	src/console.cpp
	src/key_search.cpp
	src/key_search_helpers.cpp
	src/benchmark.cpp
	src/main.cpp
)

if (WIN32 AND NOT FORCE_ANSI)
	set (PROGRAM_SOURCE ${PROGRAM_SOURCE} src/console_win32.cpp)
else()
	# *nix should have ansi support.
	set (PROGRAM_SOURCE ${PROGRAM_SOURCE} src/console_ansi.cpp)
endif()

# Configure the compiler options
set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wconversion -Wno-sign-conversion -Wextra" )
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	add_definitions( "-D_CRT_SECURE_NO_WARNINGS=1" )
endif()

# Libraries
find_package(OpenSSL 1.1 REQUIRED)
set (PROGRAM_SOURCE ${PROGRAM_SOURCE}
	src/crypto/openssl/ec.cpp
	src/crypto/openssl/hash.cpp
)

if (USE_THREADS)
	find_package(Threads)
	if (Threads_FOUND)
		# Add preprocessor flag
		add_definitions( "-DHAVE_THREADS=1" )
		set (PROGRAM_SOURCE ${PROGRAM_SOURCE} src/key_search_mt.cpp)
	endif (Threads_FOUND)
endif (USE_THREADS)

# Project config file
configure_file(src/config.h.in "${CMAKE_CURRENT_BINARY_DIR}/config.h" @ONLY)
include_directories(${PROJECT_BINARY_DIR})

# Include OpenSSL headers
include_directories( ${OPENSSL_INCLUDE_DIR} )

add_executable( ${PROGRAM_EXE} ${PROGRAM_SOURCE} )

target_link_libraries( ${PROGRAM_EXE}
	PUBLIC
	${OPENSSL_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT}
)

install(TARGETS ${PROGRAM_EXE} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# Win32 specific.
if (WIN32)
	if (CMAKE_SIZEOF_VOID_P EQUAL 8)
		set (OPENSSL_LIBCRYPTO_NAME libcrypto-1_1-x64)
	else()
		set (OPENSSL_LIBCRYPTO_NAME libcrypto-1_1)
	endif()
	set( OPENSSL_LIBCRYPTO_DLL ${OPENSSL_ROOT_DIR}/bin/${OPENSSL_LIBCRYPTO_NAME}.dll )

	# Need to copy libcrypto dll to binary folder.
	add_custom_command(TARGET ${PROGRAM_EXE} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${OPENSSL_LIBCRYPTO_DLL}
		${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}
	)

	# We also need to copy libcrypto dll during install
	install(FILES ${OPENSSL_LIBCRYPTO_DLL} DESTINATION ${CMAKE_INSTALL_BINDIR})
endif (WIN32)

# --------------------------------
#  Additional install files
# --------------------------------

install(FILES README.md LICENSE LICENSE.bitcoin
		DESTINATION ${CMAKE_INSTALL_SHAREDIR})

configure_file( docs/eosio-keygen.1.in
				${PROJECT_BINARY_DIR}/man1/eosio-keygen.1 )

install(DIRECTORY ${PROJECT_BINARY_DIR}/man1
		DESTINATION ${CMAKE_INSTALL_MANDIR})

# --------------------------------
#  Extras
# --------------------------------

if (UNIX AND EXTRAS)
	add_subdirectory(extras)
endif()

# --------------------------------
#  Misc
# --------------------------------

include( package )
