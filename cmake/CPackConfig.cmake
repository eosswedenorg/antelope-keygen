# Custom script to setup cpack properly.

# --------------------------------
#  System variables
# --------------------------------

# check CMAKE_SIZEOF_VOID_P to know if we are 32 or 64 bit.
set( CPACK_SYSTEM_ARCH "x86" )
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
	set( CPACK_SYSTEM_ARCH "${CPACK_SYSTEM_ARCH}_64" )
endif()

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")

	# Set CPACK_SYSTEM_NAME, CPACK_SYSTEM_VERSION
	# and CPACK_DEBIAN_PACKAGE_ARCHITECTURE correctly for debian based systems.
	execute_process(COMMAND lsb_release -is OUTPUT_VARIABLE SYS OUTPUT_STRIP_TRAILING_WHITESPACE)
	if (SYS)
		string(TOLOWER "${SYS}" CPACK_SYSTEM_NAME )

		# Set CPACK_SYSTEM_VERSION
		execute_process(COMMAND lsb_release -rs OUTPUT_VARIABLE SYS_VER OUTPUT_STRIP_TRAILING_WHITESPACE)
		if (SYS_VER)
			set( CPACK_SYSTEM_VERSION ${SYS_VER} )
		endif(SYS_VER)

		# Try setting CPACK_DEBIAN_PACKAGE_ARCHITECTURE based on dpkg
		if (NOT CPACK_DEBIAN_PACKAGE_ARCHITECTURE)
			execute_process(
				COMMAND dpkg --print-architecture
				OUTPUT_VARIABLE ARCH
				OUTPUT_STRIP_TRAILING_WHITESPACE
			)

			if (ARCH)
				set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${ARCH})
			endif()
		endif()
	endif(SYS)


	# set CPACK_DEBIAN_PACKAGE_ARCHITECTURE to somthing sane by default.
	if (NOT CPACK_DEBIAN_PACKAGE_ARCHITECTURE)

		# debian uses different names for 32/64 bit.
		if (CPACK_SYSTEM_ARCH STREQUAL "x86")
			set( ARCH "i386" )
		else()
			set( ARCH "amd64" )
		endif()

		set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${ARCH})
	endif()

endif()

# CPACK_SYSTEM_NAME defaults are weird. so we set good default here.
if (NOT CPACK_SYSTEM_NAME)
	set (CPACK_SYSTEM_NAME ${CMAKE_SYSTEM_NAME})
endif()

# CPACK_SYSTEM_VERSION does not exist in original CPack. so we set a default here.
if (NOT CPACK_SYSTEM_VERSION)
	set (CPACK_SYSTEM_VERSION ${CMAKE_SYSTEM_VERSION})
endif()


# --------------------------------
#  Package variables
# --------------------------------


# Forward some variables not handled by CPack.
if (NOT CPACK_PACKAGE_CONTACT AND PROJECT_MAINTAINER)
	set( CPACK_PACKAGE_CONTACT ${PROJECT_MAINTAINER} )
endif()

# Set "correct" filename that also include system version.
if (CPACK_SYSTEM_VERSION)
	set( CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}_${CPACK_SYSTEM_NAME}-${CPACK_SYSTEM_VERSION}_${CPACK_SYSTEM_ARCH}" )
else()
	set( CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}_${CPACK_SYSTEM_NAME}_${CPACK_SYSTEM_ARCH}" )
endif()


# .deb specific

# Set release version default 1.
set( CPACK_DEBIAN_PACKAGE_RELEASE "1" CACHE STRING "Debian package release version" )

# For some reason CPackDeb does not honor CPACK_PACKAGE_HOMEPAGE_URL (that is set correctly)
# if CPACK_DEBIAN_PACKAGE_HOMEPAGE (that the docs says it should.)
# So we have to do it here.
set( CPACK_DEBIAN_PACKAGE_HOMEPAGE "${PROJECT_HOMEPAGE_URL}" )

# Always build components for deb packages
set( CPACK_DEB_COMPONENT_INSTALL ON )


# --------------------------------
#  Generator default
# --------------------------------


# Default to gzip tar on unix. zip otherwise.
if (NOT CPACK_GENERATOR)
	if (UNIX)
		set( CPACK_GENERATOR "TGZ" )
	else()
		set( CPACK_GENERATOR "ZIP" )
	endif()
endif()
