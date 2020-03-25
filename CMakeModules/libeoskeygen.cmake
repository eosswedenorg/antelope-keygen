
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

macro(buildLocal src)
	message ("Using local libeoskeygen at: ${src}")
	add_subdirectory(${src} ${src}/build)
endmacro()

# If we have a local libeoskeygen
if (LIBEOSKEYGEN_SOURCE_DIR)
	buildLocal( ${LIBEOSKEYGEN_SOURCE_DIR} )
else()

	# Check if version is in fact a version.
	if (LIBEOSKEYGEN_WANTED_VERSION MATCHES "^[0-9]+(.[0-9]+)?(.[0-9]+)(-[a-zA-Z0-9]+)?$")
		# Try finding the package on the system.
		find_package(libeoskeygen ${LIBEOSKEYGEN_WANTED_VERSION} QUIET)
		if (libeoskeygen_FOUND)
			message ("Using libeoskeygen in: ${libeoskeygen_DIR}")
		# Not found, download from git.
		else()
			fromGit( v${LIBEOSKEYGEN_WANTED_VERSION} )
		endif()
	# Assume version contains a git branch.
	else()
		fromGit( ${LIBEOSKEYGEN_WANTED_VERSION} )
	endif()
endif()
