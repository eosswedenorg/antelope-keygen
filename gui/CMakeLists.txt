cmake_minimum_required(VERSION 3.15)

project(eosio-keygen-gui
	VERSION ${CMAKE_PROJECT_VERSION}
	DESCRIPTION "Keygenerator for EOSIO (gui)"
	LANGUAGES CXX)

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
	main.cpp
	MainWindow.cpp
	GenerateWindow.cpp
	SearchWindow.cpp
	MultiSelect.cpp
	helpers.cpp
)

add_executable( ${PROGRAM_EXE} WIN32 ${PROGRAM_SRC} )

# Libraries
find_package( Qt5 COMPONENTS Concurrent Core Widgets  REQUIRED )
target_link_libraries( ${PROGRAM_EXE} Qt5::Concurrent Qt5::Core Qt5::Widgets common )

# --------------------------------
#  Install
# --------------------------------

install(TARGETS ${PROGRAM_EXE}
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		COMPONENT gui)

# --------------------------------
#  Package
# --------------------------------

configure_file(cmake/CPackComponentConfig.cmake.in
	${PROJECT_BINARY_DIR}/CPackProperties.cmake)
