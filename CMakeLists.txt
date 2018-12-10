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
# add the binary tree to the search path for include files so that we will find ClothConfig.h
INCLUDE_DIRECTORIES ("${PROJECT_BINARY_DIR}")  
  
# add the head file path
INCLUDE_DIRECTORIES ("${PROJECT_SOURCE_DIR}/headers")

# add the database
INCLUDE_DIRECTORIES ("${PROJECT_SOURCE_DIR}/data")

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

# add link library
TARGET_LINK_LIBRARIES (${PROJECT_NAME})

 
