
# --------------------------------
#  Variables
# --------------------------------
set( LIBEOSKEYGEN_GIT_URL "https://github.com/eosswedenorg/libeoskeygen.git" )
set( LIBEOSKEYGEN_WANTED_VERSION 0.1.0 )

# --------------------------------
#  Macros
# --------------------------------
macro(fromGit tag)

	message ("Using libeoskeygen from: ${LIBEOSKEYGEN_GIT_URL}@${tag}")

	include(FetchContent)
	FetchContent_Declare(libeoskeygen
		GIT_REPOSITORY ${LIBEOSKEYGEN_GIT_URL}
		GIT_TAG        ${tag}
	)

	FetchContent_MakeAvailable(libeoskeygen)
endmacro()

# If we have a custom libeoskeygen
if(CUSTOM_LIBEOSKEYGEN)

	message ("Using libeoskeygen in: ${CUSTOM_LIBEOSKEYGEN}")

	# Include targets.
	include(${CUSTOM_LIBEOSKEYGEN}/libeoskeygenTargets.cmake)
else()

	# Try finding the package on the system.
	find_package(libeoskeygen ${LIBEOSKEYGEN_WANTED_VERSION} QUIET)
	if (libeoskeygen_FOUND)
		message ("Using libeoskeygen in: ${libeoskeygen_DIR}")
	# Not found, download from git.
	else()
		fromGit( v${LIBEOSKEYGEN_WANTED_VERSION} )
	endif()

endif()
