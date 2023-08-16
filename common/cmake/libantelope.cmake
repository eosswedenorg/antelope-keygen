# --------------------------------
#  Variables
# --------------------------------
set( LIBANTELOPE_GIT_URL "https://github.com/eosswedenorg/libantelope.git" )
set( LIBANTELOPE_WANTED_VERSION v0.2.2 )

# --------------------------------
#  Macros
# --------------------------------
macro(fromGit tag)

	message ("Using libantelope from: ${LIBANTELOPE_GIT_URL}@${tag}")

	include(FetchContent)
	FetchContent_Declare(libantelope
		GIT_REPOSITORY ${LIBANTELOPE_GIT_URL}
		GIT_TAG        ${tag}
		SOURCE_DIR     ${DOWNLOAD_CACHE_DIR}/libeosio/src
		STAMP_DIR      ${DOWNLOAD_CACHE_DIR}/libeosio/stamp
	)

	FetchContent_GetProperties(libantelope)
	if (NOT libantelope_POPULATED)
		FetchContent_Populate(libantelope)
		add_subdirectory(${libantelope_SOURCE_DIR} ${libantelope_BINARY_DIR} EXCLUDE_FROM_ALL)
	endif()
endmacro()

macro(buildLocal src)
	message ("Using local libantelope at: ${src}")
	add_subdirectory(${src} ${src}/build EXCLUDE_FROM_ALL)
endmacro()

# If we have a local libantelope
if (LIBANTELOPE_SOURCE_DIR)
	buildLocal( ${LIBANTELOPE_SOURCE_DIR} )
else()

	# Check if version is in fact a version.
	if (LIBANTELOPE_WANTED_VERSION MATCHES "^[0-9]+(.[0-9]+)?(.[0-9]+)(-[a-zA-Z0-9]+)?$")
		# Try finding the package on the system.
		find_package(libantelope ${LIBANTELOPE_WANTED_VERSION} QUIET)
		if (libantelope_FOUND)
			message ("Using libeosio in: ${libantelope_DIR}")
		# Not found, download from git.
		else()
			fromGit( v${LIBANTELOPE_WANTED_VERSION} )
		endif()
	# Assume version contains a git branch.
	else()
		fromGit( ${LIBANTELOPE_WANTED_VERSION} )
	endif()
endif()
