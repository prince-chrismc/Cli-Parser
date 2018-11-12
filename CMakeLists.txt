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
    add_definitions(-D_LINUX)
    set(CMAKE_CXX_STANDARD 17)

    if(CMAKE_BUILD_TYPE MATCHES Debug)
      add_compile_options(-g -D_DEBUG)
    elseif(CMAKE_BUILD_TYPE MATCHES Release)
      add_compile_options(-O3)
    else()
      set(CMAKE_BUILD_TYPE Release)
      message("Defaulting to 'Release' configuration.")
    endif()

    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        add_definitions(-D_CLANG)
    endif()

    add_compile_options(-Wall -Wextra)

    if(APPLE)
        remove_definitions(-D_LINUX)
        add_definitions(-D_DARWIN)
    endif()
elseif(WIN32)
    add_definitions(-D_WIN32)

    if(MSVC)
        add_compile_options(/std:c++17 -W4)
    else()
        message( FATAL_ERROR "Using unknown WIN32 compiler... NOT. Please add to build system." )
    endif()
endif()

# set up versioning.
set(BUILD_MAJOR "0")
set(BUILD_MINOR "0")
set(BUILD_VERSION "0")
set(BUILD_VERSION ${BUILD_MAJOR}.${BUILD_MINOR}.${BUILD_VERSION})

include_directories(src)

set(DATA_FILE README.md )
file(GLOB CLI_PARSER_HEADERS "src/*.h")
file(GLOB CLI_PARSER_SOURCES "src/*.cpp")
list(APPEND CLI_PARSER_SOURCES ${CLI_PARSER_HEADERS}) # append to sources so that dependency checks work on headers

option(BUILD_SHARED_LIBS "Build CLI Parser as a shared library." OFF)

# Build the lib and install into configured prefix
add_library(Cli-Parser  ${CLI_PARSER_SOURCES} ${DATA_FILE})
install(TARGETS Cli-Parser
                RUNTIME DESTINATION bin
                ARCHIVE DESTINATION lib )
install(FILES ${CLI_PARSER_HEADERS} DESTINATION include)
set_target_properties(Cli-Parser PROPERTIES VERSION ${BUILD_VERSION} SOVERSION ${BUILD_MAJOR})
