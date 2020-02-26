
# --------------------------------
#  Info
# --------------------------------
set( PACKAGE_NAME ${PROJECT_NAME} )
set( PACKAGE_DESCRIPTION ${PROJECT_DESCRIPTION} )
set( PACKAGE_VERSION "${PROJECT_VERSION}" )
set( PACKAGE_HOMEPAGE_URL ${PROJECT_HOMEPAGE_URL} )
set( PACKAGE_MAINTAINER ${PROJECT_MAINTAINER})

set( PACKAGE_PLATFORM "x86" )
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
	set( PACKAGE_PLATFORM "${PACKAGE_PLATFORM}_64" )
endif()

# --------------------------------
#  Debian script
# --------------------------------

if (UNIX) # Only include in bash environments.

	execute_process(
		COMMAND lsb_release -rs
		OUTPUT_VARIABLE UBUNTU_VERSION
	 	OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	# Debian uses different names for 32 and 64 bit.
	if (PACKAGE_PLATFORM EQUAL "x86")
		set( PACKAGE_DEB_PLATFORM "i386" )
	else()
		set( PACKAGE_DEB_PLATFORM "amd64" )
	endif()

	set( PACKAGE_DEB_RELEASE 1 CACHE STRING "Debian package release number")
	set( PACKAGE_DEB_VERSION ${PACKAGE_VERSION}-${PACKAGE_DEB_RELEASE} )
	set( PACKAGE_DEB_FILENAME ${PACKAGE_NAME}_${PACKAGE_DEB_VERSION}-ubuntu-${UBUNTU_VERSION}_${PACKAGE_DEB_PLATFORM}.deb )
	set( DEB_ROOT "debroot" )

	configure_file(debian_control.in ${DEB_ROOT}/DEBIAN/control @ONLY)

	add_custom_target(package_deb
		COMMAND ${CMAKE_COMMAND} --install . --prefix "${DEB_ROOT}${CMAKE_INSTALL_PREFIX}"
		COMMAND fakeroot dpkg-deb --build ${DEB_ROOT} ${PACKAGE_DEB_FILENAME} > /dev/null
		WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
	)

	add_dependencies(package_deb ${PROGRAM_EXE})
endif (UNIX)

# --------------------------------
#  Windows
# --------------------------------
if (WIN32)

	set ( PACKAGE_FILENAME "${PACKAGE_NAME}_${PACKAGE_VERSION}-windows-${PACKAGE_PLATFORM}" )

	add_custom_target(package
		COMMAND ${CMAKE_COMMAND} --install . --prefix "${PACKAGE_FILENAME}"
		COMMAND ${CMAKE_COMMAND} -E tar c "${PACKAGE_FILENAME}.zip" --format=zip "${PACKAGE_FILENAME}"
		WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
	)

	add_dependencies(package ${PROGRAM_EXE})
endif (WIN32)
