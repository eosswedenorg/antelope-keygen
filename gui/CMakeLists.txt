cmake_minimum_required(VERSION 3.15)

project(eosio-keygen-gui
	VERSION ${CMAKE_PROJECT_VERSION}
	DESCRIPTION "Keygenerator for EOSIO (gui)"
	LANGUAGES CXX)

# Append modules dir
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(QtUtils)

# Qt5 needs MOC,RCC and UIC
set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )
set( CMAKE_AUTOUIC ON )

# Autogenerate about config file

file( READ ${PROJECT_LICENSE_FILE} GUI_ABOUT_LICENSE )

string(REGEX REPLACE "^([^\n]+)" "<h2>\\1</h2>" GUI_ABOUT_LICENSE ${GUI_ABOUT_LICENSE})
string(REGEX REPLACE "\n\n([^\n]+)" "<p>\\1</p>" GUI_ABOUT_LICENSE ${GUI_ABOUT_LICENSE})
string(REGEX REPLACE "\n" "" GUI_ABOUT_LICENSE ${GUI_ABOUT_LICENSE})
string(REGEX REPLACE "\<(.+)\>" "- <a href=\"mailto:\\1\">\\1</a>" GUI_ABOUT_AUTHOR ${PROJECT_MAINTAINER})

configure_file(gui_about.h.in "${CMAKE_CURRENT_BINARY_DIR}/gui_about.h" @ONLY ESCAPE_QUOTES)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# --------------------------------
#  Program
# --------------------------------

set( PROGRAM_EXE ${PROJECT_NAME} )

set( PROGRAM_SRC
	src/main.cpp
	src/MainWindow.cpp
	src/GenerateWindow.cpp
	src/SearchWindow.cpp
	src/MultiSelect.cpp
	src/helpers.cpp
)

add_executable( ${PROGRAM_EXE} WIN32 ${PROGRAM_SRC} )

# Libraries
target_link_libraries( ${PROGRAM_EXE} common )

# QT
qt5_app(TARGET ${PROGRAM_EXE}
	SKIP_TRANSLATIONS
	MODULES Core Gui Widgets Concurrent
	INSTALL_COMPONENT gui
)

# --------------------------------
#  Install
# --------------------------------

install(TARGETS ${PROGRAM_EXE}
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		COMPONENT gui)

install(FILES ${PROJECT_LICENSE_FILE}
		DESTINATION ${CMAKE_INSTALL_SHAREDIR}
		COMPONENT gui
		RENAME LICENSE.gui)

# --------------------------------
#  Package
# --------------------------------

configure_file(cmake/CPackComponentConfig.cmake.in
	${PROJECT_BINARY_DIR}/CPackProperties.cmake)
