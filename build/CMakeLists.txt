cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

########################################################
# Find all necessary libraries                         #
########################################################

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON) 
set(Boost_USE_STATIC_RUNTIME OFF)

project(god)

find_package(Boost COMPONENTS system thread)
find_package (Threads)

set(Boost_DIR $ENV{BOOST_ROOT})
add_definitions( -DBOOST_ALL_NO_LIB )
add_definitions( -DBOOST_ALL_DYN_LINK )
########################################################
# Define project                                       #
########################################################



set(god_VERSION_MAJOR 1)
set(god_VERSION_MINOR 0)

set(projectRoot "${CMAKE_CURRENT_SOURCE_DIR}/..")

###############################################
# Library of code common to client and server #
###############################################

set(commonRoot ${projectRoot}/common)
set(commonSrc ${commonRoot}/src)
configure_file(
      "${commonSrc}/config.h.in"
      "${commonSrc}/config.h"
   )

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
include_directories(
      ${serverSrc}
      ${commonSrc}
      ${Boost_INCLUDE_DIRS}
   )

add_executable(
      ${serverName}
      ${serverSrcFiles}
   )
target_link_libraries(
      ${serverName}
      ${commonLibraryName}
      ${Boost_LIBRARIES}
      ${CMAKE_THREAD_LIBS_INIT}
   )

###############################################
# Client application                          #
###############################################

set(clientName "god_client")
set(clientRoot ${projectRoot}/client)
set(clientSrc ${clientRoot}/src)
file(GLOB clientSrcFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${clientSrc}/*.cpp)
include_directories(
      ${clientSrc}
      ${commonSrc}
      ${Boost_INCLUDE_DIRS}
   )
add_executable(
      ${clientName}
      ${clientSrcFiles}
   )
target_link_libraries(
      ${clientName}
      ${commonLibraryName}
      ${Boost_LIBRARIES}
      ${CMAKE_THREAD_LIBS_INIT}
   )

