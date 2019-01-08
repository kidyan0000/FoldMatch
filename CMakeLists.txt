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

# add the head file path
SET( HEADERS "${PROJECT_SOURCE_DIR}/headers/cloth_glwidget.h"
             "${PROJECT_SOURCE_DIR}/headers/cloth_window.h"
             "${PROJECT_SOURCE_DIR}/headers/ply_Module.h"
             "${PROJECT_SOURCE_DIR}/headers/tinyply.h")
# add the source file path
SET( SRC "${PROJECT_SOURCE_DIR}/src/cloth_glwidget"
        "${PROJECT_SOURCE_DIR}/src/cloth_window.cpp"
        "${PROJECT_SOURCE_DIR}/src/main.cpp"
        "${PROJECT_SOURCE_DIR}/src/ply_Module.cpp"
        "${PROJECT_SOURCE_DIR}/src/tinyply.cpp")
# add the gui file path
SET( GUI "${PROJECT_SOURCE_DIR}/GUI/cloth_window.ui")

# add current dictory
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

#
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTORCC ON)

# ADD_LIBRARY(LIB SHARED ${MOC_FILES} ${UIC_FILES})

# Find the QtWidgets library
FIND_PACKAGE(Qt5Widgets CONFIG REQUIRED)
FIND_PACKAGE(Qt5Core)
FIND_PACKAGE(Qt5Gui)
FIND_PACKAGE(Qt5OpenGL)

# QT5_USE_MODULES(${PROJECT_NAME} Core Widgets Gui OpenGL)
# SET(QT_USE_QTOPENGL TRUE)

# SET(CMAKE_AUTOMOC_RELAXED_MODE TRUE)
QT5_WRAP_CPP(MOC_FILES ${HEADERS})
QT5_WRAP_UI(UIC_FILES ${GUI})

# add the binary tree to the search path for include files so that we will find ClothConfig.h
INCLUDE_DIRECTORIES ("${PROJECT_BINARY_DIR}")   

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
# AUX_SOURCE_DIRECTORY("${PROJECT_SOURCE_DIR}/src" DIR_SRCS)
# SET(CLOTH_SRCS ${DIR_SRCS})
 
# add the executable
ADD_EXECUTABLE (${PROJECT_NAME} ${SRC} ${MOC_FILES} ${UIC_FILES})

# Use the Widgets module from Qt 5
TARGET_LINK_LIBRARIES (${PROJECT_NAME} Qt5::Widgets)
