cmake_minimum_required(VERSION 3.4)
project(eosio-keygen VERSION 0.1.0)

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

set( INSTALL_BIN_DIR bin )
set( INSTALL_SHARE_DIR share/${CMAKE_PROJECT_NAME} )

find_package(OpenSSL 1.1 REQUIRED)

include_directories( ${OPENSSL_INCLUDE_DIR} )

add_executable( ${CMAKE_PROJECT_NAME}
	src/string.cpp
	src/ec.cpp
	src/base58.cpp
	src/checksum.cpp
	src/WIF.cpp
	src/main.cpp
)

target_link_libraries( ${CMAKE_PROJECT_NAME} PUBLIC ${OPENSSL_LIBRARIES} )

if (WIN32)
	if (CMAKE_SIZEOF_VOID_P EQUAL 8)
		set (OPENSSL_LIBCRYPTO_NAME libcrypto-1_1-x64)
	else()
		set (OPENSSL_LIBCRYPTO_NAME libcrypto-1_1)
	endif()
	set( OPENSSL_LIBCRYPTO_DLL ${OPENSSL_ROOT_DIR}/bin/${OPENSSL_LIBCRYPTO_NAME}.dll )

	# Need to copy libcrypto dll to binary folder.
	add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${OPENSSL_LIBCRYPTO_DLL}
		${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}
	)

	# "Flat" install on windows.
	set( INSTALL_BIN_DIR "." )
	set( INSTALL_SHARE_DIR "." )

	# We also need to copy libcrypto dll during install
	install(FILES ${OPENSSL_LIBCRYPTO_DLL} DESTINATION ${INSTALL_BIN_DIR})
endif (WIN32)

# Debian .deb generation script
if (UNIX) # Only include in bash environments.
	# Hack to set the right permissions :)
	configure_file(scripts/generate_deb.sh.in ${PROJECT_BINARY_DIR}/tmp/generate_deb.sh @ONLY)
	file(COPY ${PROJECT_BINARY_DIR}/tmp/generate_deb.sh DESTINATION ${PROJECT_BINARY_DIR}
	    FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
	)
endif (UNIX)

install(TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION ${INSTALL_BIN_DIR})
install(FILES README.md LICENSE LICENSE.bitcoin DESTINATION ${INSTALL_SHARE_DIR})
