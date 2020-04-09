
# Qt5 needs MOC,RCC and UIC
set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )
set( CMAKE_AUTOUIC ON )

# --------------------------------
#  Program
# --------------------------------

set( PROGRAM_EXE ${CMAKE_PROJECT_NAME}-gui )

set( PROGRAM_SRC
	main.cpp
	SearchWindow.cpp
)

add_executable( ${PROGRAM_EXE} ${PROGRAM_SRC} )

# Libraries
find_package( Qt5 5.11 COMPONENTS Core Widgets REQUIRED )
target_link_libraries( ${PROGRAM_EXE} Qt5::Core Qt5::Widgets common )
