cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

project(god)

# Set version information in a config.h file
#set(myproject_VERSION_MAJOR 1)
#set(myproject_VERSION_MINOR 0)
#configure_file(
#  "${PROJECT_SOURCE_DIR}/config.h.in"
#  "${PROJECT_BINARY_DIR}/config.h"
#  )

set(projectRoot "../..")

###############################################
# Library of code common to client and server #
###############################################

set(commonRoot ${projectRoot}/common)
set(commonSrc ${commonRoot}/src)
file(GLOB commonSrcFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${commonSrc}/*.cpp)
include_directories(${commonSrc})
set(commonLibraryName "god_common")
add_library(
      ${commonLibraryName}
      SHARED
      ${commonSrcFiles}
   )

###############################################
# Server application                          #
###############################################

set(serverName "god_server")
set(serverRoot ${projectRoot}/server)
set(serverSrc ${serverRoot}/src)
file(GLOB serverSrcFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${serverSrc}/*.cpp)
include_directories(${serverSrc} ${commonSrc})
add_executable(
      ${serverName}
      ${serverSrcFiles}
   )
target_link_libraries(${serverName} ${commonLibraryName})

###############################################
# Client application                          #
###############################################

set(clientName "god_client")
set(clientRoot ${projectRoot}/client)
set(clientSrc ${clientRoot}/src)
file(GLOB clientSrcFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${clientSrc}/*.cpp)
include_directories(${clientSrc} ${commonSrc})
add_executable(
      ${clientName}
      ${clientSrcFiles}
   )
target_link_libraries(${clientName} ${commonLibraryName})

