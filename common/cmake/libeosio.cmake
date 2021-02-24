# --------------------------------
#  Variables
# --------------------------------
set( LIBEOSIO_GIT_URL "https://github.com/eosswedenorg/libeosio.git" )
set( LIBEOSIO_WANTED_VERSION v0.1.2 )

# --------------------------------
#  Macros
# --------------------------------
macro(fromGit tag)

	message ("Using libeosio from: ${LIBEOSIO_GIT_URL}@${tag}")

	include(FetchContent)
	FetchContent_Declare(libeosio
		GIT_REPOSITORY ${LIBEOSIO_GIT_URL}
		GIT_TAG        ${tag}
		SOURCE_DIR     ${DOWNLOAD_CACHE_DIR}/libeosio/src
		STAMP_DIR	   ${DOWNLOAD_CACHE_DIR}/libeosio/stamp
	)

	FetchContent_GetProperties(libeosio)
	if (NOT libeosio_POPULATED)
		FetchContent_Populate(libeosio)
		add_subdirectory(${libeosio_SOURCE_DIR} ${libeosio_BINARY_DIR} EXCLUDE_FROM_ALL)
	endif()
endmacro()

macro(buildLocal src)
	message ("Using local libeosio at: ${src}")
	add_subdirectory(${src} ${src}/build EXCLUDE_FROM_ALL)
endmacro()

# If we have a locallibeosio
if (LIBEOSIO_SOURCE_DIR)
	buildLocal( ${LIBEOSIO_SOURCE_DIR} )
else()

	# Check if version is in fact a version.
	if (LIBEOSIO_WANTED_VERSION MATCHES "^[0-9]+(.[0-9]+)?(.[0-9]+)(-[a-zA-Z0-9]+)?$")
		# Try finding the package on the system.
		find_package(libeosio ${LIBEOSIO_WANTED_VERSION} QUIET)
		if (libeoskeygen_FOUND)
			message ("Using libeosio in: ${libeosio_DIR}")
		# Not found, download from git.
		else()
			fromGit( v${LIBEOSIO_WANTED_VERSION} )
		endif()
	# Assume version contains a git branch.
	else()
		fromGit( ${LIBEOSIO_WANTED_VERSION} )
	endif()
endif()
