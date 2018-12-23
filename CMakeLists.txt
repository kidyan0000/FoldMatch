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
SET(CLOTH_SRCS ${DIR_SRCS})
 
# add the executable
ADD_EXECUTABLE (${PROJECT_NAME} ${CLOTH_SRCS})


# Use the Widgets module from Qt 5
TARGET_LINK_LIBRARIES (${PROJECT_NAME} Qt5::Widgets)








# set (LIBNAME kissqt)
# 
# file (GLOB HEADERS
# 	  "*.h"
# 	  "*.hpp")
# 
# file (GLOB SOURCES "*.cpp")
# file (GLOB UI_FILES "*.ui")
# 
# # Create named folders for the sources within the .vcproj
# # Empty name lists them directly under the .vcproj
# source_group("headers" FILES ${HEADERS})
# source_group("src" FILES ${SOURCES})
# source_group("ui" FILES ${UI_FILES})
# 
# # Properties->C/C++->General->Additional Include Directories
# include_directories (.)
# include_directories (..)
# include_directories (/home/ksoft/Qt5.6.0/5.6/gcc_64/include/)
# 
# set(CMAKE_AUTOMOC_RELAXED_MODE TRUE)
# 
# find_package(Qt5Widgets)
# QT5_WRAP_CPP(tst_hdr_moc ${HEADERS})
# QT5_WRAP_UI(tst_form_hdr ${UI_FILES})
# set(CMAKE_INCLUDE_CURRENT_DIR ON)
# 
# # Set Properties->General->Configuration Type to Dynamic Library(.dll)
# # Creates math.dll with the listed sources collected in the variables
# # Also adds sources to the Solution Explorer
# add_library(${LIBNAME} SHARED ${tst_hdr_moc} ${tst_form_hdr})
# 
# set_target_properties(${LIBNAME} PROPERTIES 
# 	AUTOMOC ON
# 	AUTOUIC ON
# 	DEFINE_SYMBOL "KISSQT_LIB"
# 	OUTPUT_NAME ${LIBNAME}${LIB_END}
# 	OUTPUT_NAME_DEBUG ${LIBNAME}${LIB_END_DEBUG}
# 	OUTPUT_NAME_RELEASE ${LIBNAME}${LIB_END_RELEASE})
# 
# # Set the library dependencies
# qt5_use_modules(${LIBNAME} Core Widgets Gui OpenGL Designer)
# 
# # Creates a folder "libraries" and adds target project (math.vcproj) under it
# set_property(TARGET ${LIBNAME} PROPERTY FOLDER "lib")
# 
# # Adds logic to INSTALL.vcproj to copy math.dll to the destination directory
# install (TARGETS ${LIBNAME}
# 		 RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin
# 		 LIBRARY DESTINATION ${PROJECT_BINARY_DIR}/bin)

 
