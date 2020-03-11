cmake_minimum_required(VERSION 3.15)

# --------------------------------
#  Program
# --------------------------------

set (PROGRAM_EXE ${CMAKE_PROJECT_NAME})

set (PROGRAM_SOURCE
	src/core/file.cpp
	src/core/dictionary.cpp
	src/core/string.cpp
	src/core/isatty.cpp
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
configure_file(src/config.h.in "${PROJECT_BINARY_DIR}/config.h" @ONLY)
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

include( package )
