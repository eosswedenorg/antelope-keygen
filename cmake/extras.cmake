
set( EXTRAS_DIRECTORY ${DOWNLOAD_CACHE_DIR}/eosio-keygen-extras-0.1.0 )
set( FILENAME ${DOWNLOAD_CACHE_DIR}/eosio-keygen-extras-0.1.0.zip )

# --------------------------------
#  Download/Unpack
# --------------------------------

if (NOT EXISTS ${FILENAME})

	set( URL "https://github.com/eosswedenorg/eosio-keygen-extras/releases/download/v0.1.0/eosio-keygen-extras-0.1.0.zip" )
	set( CHECKSUM "7be3188a52a39876e37986e6a7d78e0d6c89e68e8391cf48821c800563aaa036" )

	message( STATUS "Downloading ${URL}" )

	file(DOWNLOAD ${URL} ${FILENAME}
	   TIMEOUT 60
	   EXPECTED_HASH SHA256=${CHECKSUM}
	   TLS_VERIFY ON)
endif()

if (NOT EXISTS ${EXTRAS_DIRECTORY})

	message( STATUS "Unpacking ${FILENAME}" )

	execute_process(COMMAND ${CMAKE_COMMAND} -E tar -xf ${FILENAME}
					WORKING_DIRECTORY ${DOWNLOAD_CACHE_DIR})
endif()


# --------------------------------
#  Install
# --------------------------------

install(DIRECTORY ${EXTRAS_DIRECTORY}/dict
		DESTINATION ${CMAKE_INSTALL_SHAREDIR}
		COMPONENT extras )

# --------------------------------
#  CPack
# --------------------------------

set( CPACK_COMPONENT_EXTRAS_DESCRIPTION "Dictionary files" )
