
# If we have a custom libeoskeygen
if(CUSTOM_LIBEOSKEYGEN)

	message ("Using libeoskeygen in: ${CUSTOM_LIBEOSKEYGEN}")

	# Include targets.
	include(${CUSTOM_LIBEOSKEYGEN}/libeoskeygenTargets.cmake)
else()

	set( LIBEOSKEYGEN_WANTED_VERSION 0.1.0 )

	# Try finding the package on the system.
	find_package(libeoskeygen ${LIBEOSKEYGEN_WANTED_VERSION} QUIET)
	if (libeoskeygen_FOUND)
		message ("Using libeoskeygen in: ${libeoskeygen_DIR}")
	# Not found, download from git.
	else()
		message ("Using libeoskeygen from: https://github.com/eosswedenorg/libeoskeygen.git@v${LIBEOSKEYGEN_WANTED_VERSION}")

		include(FetchContent)
		FetchContent_Declare(
			libeoskeygen
			GIT_REPOSITORY https://github.com/eosswedenorg/libeoskeygen.git
			GIT_TAG        v${LIBEOSKEYGEN_WANTED_VERSION}
		)

		FetchContent_MakeAvailable(libeoskeygen)
	endif()

endif()
