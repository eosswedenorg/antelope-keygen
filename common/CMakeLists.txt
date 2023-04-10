# ------------------------------------------------------------
#  Common CMake file
#
#  Compiles the code that should be shared between the cli
#  and gui programs into a static library.
# ------------------------------------------------------------

# Options
option(USE_THREADS "Compile with support for threads (if available)." ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# --------------------------------
#  Library
# --------------------------------
set( COMMON_NAME common )

set( COMMON_SOURCE
	src/core/file.cpp
	src/core/dictionary.cpp
	src/core/string.cpp
	src/core/strlist.cpp
	src/core/leet.cpp
	src/key_search.cpp
)

# Threads support
if (USE_THREADS)
	find_package(Threads)
	if (Threads_FOUND)
		set( ANTELOPEKEYGEN_HAVE_THREADS TRUE )
		set( COMMON_SOURCE ${COMMON_SOURCE} src/key_search_mt.cpp )
	endif (Threads_FOUND)
endif (USE_THREADS)

# Project config file
configure_file(config.hpp.in "${CMAKE_CURRENT_LIST_DIR}/include/eoskeygen/config.hpp" @ONLY)

add_library( ${COMMON_NAME} STATIC ${COMMON_SOURCE} )

target_include_directories( ${COMMON_NAME} PUBLIC include )

# Link with libantelope and threads library.
include( libantelope )
target_link_libraries( ${COMMON_NAME}
	PUBLIC
		libantelope
		${CMAKE_THREAD_LIBS_INIT}
)
