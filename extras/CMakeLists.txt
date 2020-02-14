cmake_minimum_required(VERSION 3.4)

# Project name and version
project(eosio-keygen-extras VERSION 0.1.0)

# --------------------------------
#  Package info
# --------------------------------
set( PACKAGE_NAME ${PROJECT_NAME} )
set( PACKAGE_DESCRIPTION "Extra files for eosio-keygen" )
set( PACKAGE_VERSION "${PROJECT_VERSION}" )
set( PACKAGE_URL "https://github.com/eosswedenorg/eosio-keygen" )
set( PACKAGE_MAINTAINER "Henrik Hautakoski <henrik@eossweden.org>")

# Debian specific
set( PACKAGE_DEB_RELEASE 1 CACHE STRING "Debian package release number")
set( PACKAGE_DEB_VERSION ${PACKAGE_VERSION}-${PACKAGE_DEB_RELEASE} )
set( PACKAGE_DEB_FILENAME ${PACKAGE_NAME}_${PACKAGE_DEB_VERSION}.deb )

# --------------------------------
#  Debian package
# --------------------------------
set( DEB_ROOT "debroot" )
set( DEB_FULL_ROOT ${CMAKE_CURRENT_BINARY_DIR}/${DEB_ROOT} )
set( DEB_SHARE_DIR ${CMAKE_INSTALL_FULL_DATADIR}/${CMAKE_PROJECT_NAME} )

configure_file(debian_control.in ${DEB_ROOT}/DEBIAN/control @ONLY)

add_custom_target(deb_content
    COMMAND ${CMAKE_COMMAND} -E copy_directory
		dict
		${DEB_FULL_ROOT}/${DEB_SHARE_DIR}/dict
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(deb_extras
	COMMENT "dpkg: ${PROJECT_NAME} (${PACKAGE_DEB_FILENAME})"
	COMMAND fakeroot dpkg-deb --build ${DEB_ROOT} ${PACKAGE_DEB_FILENAME} > /dev/null
	WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
	DEPENDS deb_content
)

# --------------------------------
#  Install target
# --------------------------------
install(
	DIRECTORY dict
	DESTINATION ${CMAKE_INSTALL_DATADIR}/${CMAKE_PROJECT_NAME}
)
