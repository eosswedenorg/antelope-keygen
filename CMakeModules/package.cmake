
# --------------------------------
#  Info
# --------------------------------
set( PACKAGE_NAME ${PROJECT_NAME} )
set( PACKAGE_DESCRIPTION ${PROJECT_DESCRIPTION} )
set( PACKAGE_VERSION "${PROJECT_VERSION}" )
set( PACKAGE_HOMEPAGE_URL ${PROJECT_HOMEPAGE_URL} )
set( PACKAGE_MAINTAINER ${PROJECT_MAINTAINER})

# --------------------------------
#  Debian script
# --------------------------------

if (UNIX) # Only include in bash environments.
	# Hack to set the right permissions :)
	configure_file(scripts/generate_deb.sh.in ${PROJECT_BINARY_DIR}/tmp/generate_deb.sh @ONLY)
	file(COPY ${PROJECT_BINARY_DIR}/tmp/generate_deb.sh DESTINATION ${PROJECT_BINARY_DIR}
	    FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
	)
endif (UNIX)
