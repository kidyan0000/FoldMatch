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
INCLUDE_DIRECTORIES ("${PROJECT_SOURCE_DIR}/headers")
SET( HEADERS "${PROJECT_SOURCE_DIR}/headers")
# add the database path
# INCLUDE_DIRECTORIES ("${PROJECT_SOURCE_DIR}/data")
# SET( DATA "${PROJECT_SOURCE_DIR}/data")
# add the GUI file path
# INCLUDE_DIRECTORIES ("${PROJECT_SOURCE_DIR}/GUI")
# SET( GUI "${PROJECT_SOURCE_DIR}/GUI")
# add the source file path
# INCLUDE_DIRECTORIES ("${PROJECT_SOURCE_DIR}/src")
# SET( SRC "${PROJECT_SOURCE_DIR}/src")

#
# QT5_WRAP_CPP( MOC ${HEADERS}/clothwindow.h)
# QT5_WRAP_UI( UIC ${GUI}/clothwindow.ui)

# set user include
SET(INCLUDE_DIR "/usr/include")
# add some optinal libraries
OPTION(USE_OPT "Use user defined provided libraries" ON)
IF (USE_OPT)
# include EIGEN libraries
 SET( EIGEN3_INCLUDE_DIR "${INCLUDE_DIR}/eigen3")
 INCLUDE_DIRECTORIES ( "${EIGEN3_INCLUDE_DIR}" )
ENDIF (USE_OPT)

# add the source diretory
AUX_SOURCE_DIRECTORY("${PROJECT_SOURCE_DIR}/src" DIR_SRCS)
# add to enviroment variable
SET(CLOTH_SRCS ${DIR_SRCS})
 
# add the executable
ADD_EXECUTABLE (${PROJECT_NAME} ${CLOTH_SRCS})

# Use the Widgets module from Qt 5
TARGET_LINK_LIBRARIES (${PROJECT_NAME} Qt5::Widgets)

 
