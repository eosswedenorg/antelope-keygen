
# Qt5 needs MOC,RCC and UIC
set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )
set( CMAKE_AUTOUIC ON )

# --------------------------------
#  Program
# --------------------------------

set( COMPONENT_NAME ${CMAKE_PROJECT_NAME}-gui)

set( PROGRAM_EXE ${COMPONENT_NAME} )

set( PROGRAM_SRC
	main.cpp
	GenerateWindow.cpp
	SearchWindow.cpp
	MultiSelect.cpp
	helpers.cpp
)

add_executable( ${PROGRAM_EXE} ${PROGRAM_SRC} )

# Libraries
find_package( Qt5 COMPONENTS Concurrent Core Widgets  REQUIRED )
target_link_libraries( ${PROGRAM_EXE} Qt5::Concurrent Qt5::Core Qt5::Widgets common )

# --------------------------------
#  Install
# --------------------------------

install(TARGETS ${PROGRAM_EXE}
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		COMPONENT ${COMPONENT_NAME})
