
set(_QT_INSTALL_CONFIG_TEMPLATE ${CMAKE_CURRENT_LIST_DIR}/cmake_install_qt.cmake.in)
set(_QT_INSTALL_CONFIG ${CMAKE_CURRENT_BINARY_DIR}/cmake_install_qt.cmake)

# ------------------------------------------------------------------------------
#  Code Taken and modified from:
#  https://github.com/equalsraf/neovim-qt/blob/master/cmake/WinDeployQt.cmake
#
#  Wrapper to call windeployqt on Windows
# ------------------------------------------------------------------------------
function(qt5_win_deploy)
	cmake_parse_arguments(_deploy
		"COMPILER_RUNTIME;FORCE;SKIP_TRANSLATIONS"
		"TARGET;INSTALL_COMPONENT"
		"MODULES;EXCLUDE_MODULES"
		${ARGN}
		)

	if(NOT _deploy_TARGET)
		message(FATAL_ERROR "A TARGET must be specified")
	endif()
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		list(APPEND _ARGS --debug)
	elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
		list(APPEND _ARGS --release-with-debug-info)
	elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
		list(APPEND _ARGS --release)
	endif()
	if(_deploy_COMPILER_RUNTIME)
		list(APPEND _ARGS --compiler-runtime)
	endif()
	if(_deploy_FORCE)
		list(APPEND _ARGS --force)
	endif()
	if(_deploy_SKIP_TRANSLATIONS)
		list(APPEND _ARGS --no-translations)
	endif()

	foreach(mod ${_deploy_MODULES})
		string(TOLOWER ${mod} mod)
		string(REPLACE "qt5::" "" mod ${mod})
		list(APPEND _ARGS "--${mod}")
	endforeach()
	foreach(mod ${_deploy_EXCLUDE_MODULES})
		string(TOLOWER ${mod} mod)
		string(REPLACE "qt5::" "" mod ${mod})
		list(APPEND _ARGS "--no-${mod}")
	endforeach()

	find_program(_deploy_PROGRAM windeployqt PATHS $ENV{QTDIR}/bin)
	if(_deploy_PROGRAM)
		message(STATUS "Found ${_deploy_PROGRAM}")
	else()
		message(FATAL_ERROR "Unable to find windeployqt")
	endif()

	if(COMPILER_RUNTIME AND NOT $ENV{VVVV})
		message(STATUS "not set, the VC++ redistributable installer will NOT be bundled")
	endif()

	set( _QT_INSTALL_CONFIG_DEPFILE "${CMAKE_CURRENT_BINARY_DIR}/qt_deps_\${CMAKE_INSTALL_CONFIG_NAME}.cmake" )
	set( _QT_DEPFILE ${CMAKE_CURRENT_BINARY_DIR}/qt_deps_${CMAKE_BUILD_TYPE}.cmake )

	add_custom_target(${_deploy_TARGET}_windeployqt ALL
		COMMAND ${CMAKE_COMMAND} -E echo "$<TARGET_FILE_DIR:${_deploy_TARGET}>" > ${_QT_DEPFILE}
		COMMAND ${_deploy_PROGRAM} --list relative ${_ARGS} $<TARGET_FILE:${_deploy_TARGET}> >> ${_QT_DEPFILE}
		DEPENDS ${_deploy_TARGET}
		COMMENT "Preparing Qt runtime dependencies")

	configure_file(${_QT_INSTALL_CONFIG_TEMPLATE} ${_QT_INSTALL_CONFIG} @ONLY)

	if (_deploy_INSTALL_COMPONENT)
		set(_install_args "COMPONENT ${_deploy_INSTALL_COMPONENT}")
	endif()

	install(SCRIPT ${_QT_INSTALL_CONFIG} ${_install_args})
endfunction()

# ------------------------------------------------------------------------------
#  Macro for compile, link and deploy qt apps.
# ------------------------------------------------------------------------------
macro(qt5_app)
	cmake_parse_arguments(_args
		"COMPILER_RUNTIME;FORCE;SKIP_TRANSLATIONS"
		"TARGET;INSTALL_COMPONENT"
		"MODULES"
		${ARGN}
	)

	if(NOT _args_TARGET)
		message(FATAL_ERROR "A TARGET must be specified")
	endif()

	if(NOT _args_MODULES)
		message(FATAL_ERROR "Must define atleast one QT component")
	endif()

	find_package( Qt5 COMPONENTS ${_args_MODULES} REQUIRED)

	# Link targets
	list( TRANSFORM _args_MODULES PREPEND "Qt5::" OUTPUT_VARIABLE _qtargets)
	target_link_libraries( ${_args_TARGET} ${_qtargets} )


	if (WIN32)
		# Windows needs alot of extra stuff to copy all dll's.
		qt5_win_deploy(${ARGN})
	endif()

endmacro()
