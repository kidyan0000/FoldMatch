CMAKE_MINIMUM_REQUIRED (VERSION 3.13.0)
PROJECT (CLOTH_SIMULATION)

# The version number
SET (CLOTH_SIMULATION_VERSION_MAJOR 1)
SET (CLOTH_SIMULATION_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code
CONFIGURE_FILE (
  "${PROJECT_SOURCE_DIR}/ClothConfig.h.in"
  "${PROJECT_BINARY_DIR}/ClothConfig.h"
)

# add current dictory
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

#
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTORCC ON)

# Find the QtWidgets library
FIND_PACKAGE(Qt5Widgets CONFIG REQUIRED)
FIND_PACKAGE(Qt5Core)
FIND_PACKAGE(Qt5Gui)
FIND_PACKAGE(Qt5OpenGL)
  
# add the binary tree to the search path for include files so that we will find ClothConfig.h
INCLUDE_DIRECTORIES ("${PROJECT_BINARY_DIR}")   
# add the head file path
INCLUDE_DIRECTORIES ("${PROJECT_SOURCE_DIR}")


# set user include
SET(INCLUDE_DIR "/usr/include")
# add some optinal libraries
OPTION(USE_OPT "Use user defined provided libraries" ON)
IF (USE_OPT)
# include EIGEN libraries
 SET( EIGEN3_INCLUDE_DIR "${INCLUDE_DIR}/eigen3")
 INCLUDE_DIRECTORIES ( "${EIGEN3_INCLUDE_DIR}" )
ENDIF (USE_OPT)

# SET(HEADERS "*.h")
# SET(SRC "*.cpp")
 
# add the executable
ADD_EXECUTABLE (${PROJECT_NAME} main.cpp qt_example.cpp Circle.cpp qt_example.ui)

# add link library
TARGET_LINK_LIBRARIES (${PROJECT_NAME} Qt5::Widgets)
 
