cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(Cli-Parser)

# CMake Settings
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif ()

#set(CMAKE_VERBOSE_MAKEFILE ON)

# OS and compiler checks.
if(UNIX)
    # linux / normal unix
    add_definitions(-D_LINUX)
    set(CMAKE_CXX_STANDARD 17)
    if(SIMPLE_SOCKET_DEP_ONLY)
       if(CMAKE_BUILD_TYPE MATCHES Debug)
         message("Building 'Debug' configuration.")
         set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -D_DEBUG")
       elseif(CMAKE_BUILD_TYPE MATCHES Release)
         message("Building 'Release' configuration.")
       else()
         set(CMAKE_BUILD_TYPE Release)
         message("Defaulting to 'Release' configuration.")
       endif()
    endif()

    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        add_definitions(-D_CLANG)
    endif()

    if(CYGWIN)
        # Special Cygwin stuff here
    elseif(APPLE)
        # Special Apple stuff here
        remove_definitions(-D_LINUX)
        add_definitions(-D_DARWIN)
    endif()
elseif(WIN32)
    add_definitions(-D_WIN32)
    if(MINGW)
        # Special MINGW stuff here
    elseif(MSVC)
        # Special MSVC stuff here
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
    else()
        # No idea what it is, but there's a good chance it's too weird.
        MESSAGE( FATAL_ERROR "Using unknown WIN32 compiler... NOT. Please add to build system." )
    endif()
endif()

OPTION(CLI_PARSER_DEP_ONLY "Build for use inside other CMake projects as dependency." OFF)

# set up versioning.
set(BUILD_MAJOR "0")
set(BUILD_MINOR "0")
set(BUILD_VERSION "0")
set(BUILD_VERSION ${BUILD_MAJOR}.${BUILD_MINOR}.${BUILD_VERSION})

include_directories(src)

set(DATA_FILE README.md )
FILE(GLOB CLI_PARSER_HEADERS "src/*.h")
FILE(GLOB CLI_PARSER_SOURCES "src/*.cpp")

SET_SOURCE_FILES_PROPERTIES( ${CLI_PARSER_HEADERS} PROPERTIES HEADER_FILE_ONLY TRUE ) # mark headers as headers...
LIST(APPEND CLI_PARSER_SOURCES ${CLI_PARSER_HEADERS}) # append to sources so that dependency checks work on headers

# Build the lib and install into configured prefix
if(CLI_PARSER_DEP_ONLY)
    ADD_LIBRARY(Cli-Parser STATIC ${CLI_PARSER_SOURCES} ${DATA_FILE})
    install(TARGETS Cli-Parser
                    LIBRARY DESTINATION lib
                    ARCHIVE DESTINATION lib/${BUILD_VERSION} )
    install(FILES ${CLI_PARSER_HEADERS} DESTINATION include)
else()
    ADD_LIBRARY(Cli-Parser STATIC ${CLI_PARSER_SOURCES})
endif()

set_target_properties(Cli-Parser PROPERTIES VERSION ${BUILD_VERSION} SOVERSION ${BUILD_MAJOR})
