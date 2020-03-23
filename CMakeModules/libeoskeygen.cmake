
# If we have a custom libeoskeygen
if(CUSTOM_LIBEOSKEYGEN)

	message ("Using libeoskeygen in: ${CUSTOM_LIBEOSKEYGEN}")

	# Include targets.
	include(${CUSTOM_LIBEOSKEYGEN}/libeoskeygenTargets.cmake)
else()

	# TODO: find_package() here if libeoskeygen is installed on the system.
	# otherwise we will download with FetchContent.

	message ("Using libeoskeygen from: https://github.com/eosswedenorg/libeoskeygen.git")

	include(FetchContent)
	FetchContent_Declare(
		libeoskeygen
		GIT_REPOSITORY https://github.com/eosswedenorg/libeoskeygen.git
		GIT_TAG        master
	)

	FetchContent_MakeAvailable(libeoskeygen)

endif()
