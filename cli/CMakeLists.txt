# Options
option(FORCE_ANSI "Force ANSI console colors even on windows" OFF)

# --------------------------------
#  Program
# --------------------------------

set (PROGRAM_EXE ${CMAKE_PROJECT_NAME})

set (PROGRAM_SOURCE
	isatty.cpp
	cli_key_search_result.cpp
	console.cpp
	benchmark.cpp
	main.cpp
)

if (WIN32 AND NOT FORCE_ANSI)
	set (PROGRAM_SOURCE ${PROGRAM_SOURCE} console_win32.cpp)
else()
	# *nix should have ansi support.
	set (PROGRAM_SOURCE ${PROGRAM_SOURCE} console_ansi.cpp)
endif()

add_executable( ${PROGRAM_EXE} ${PROGRAM_SOURCE} )

target_link_libraries( ${PROGRAM_EXE} PUBLIC common )

# --------------------------------
#  Install
# --------------------------------

install(TARGETS ${PROGRAM_EXE} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# Documentation

if (UNIX)
	configure_file( docs/eosio-keygen.1.in ${PROJECT_BINARY_DIR}/man1/eosio-keygen.1 )

	install(DIRECTORY ${PROJECT_BINARY_DIR}/man1
			DESTINATION ${CMAKE_INSTALL_MANDIR})
endif (UNIX)
