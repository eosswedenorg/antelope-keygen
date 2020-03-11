
if (UNIX)
	configure_file( eosio-keygen.1.in ${PROJECT_BINARY_DIR}/man1/eosio-keygen.1 )

	install(DIRECTORY ${PROJECT_BINARY_DIR}/man1
			DESTINATION ${CMAKE_INSTALL_MANDIR})
endif (UNIX)
