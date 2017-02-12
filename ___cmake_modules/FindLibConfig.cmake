#.rst:
# FindLibConfig
# -------
# - Find LibConfig++ library
#
# Use this module by invoking find_package with the form::
#
#   find_package(LibConfig
#     [version] [EXACT]      # Minimum or EXACT version e.g. 1.2.0
#     [REQUIRED]             # Fail with error if LibConfig not found
#     )
#
# This module defines
#  LibConfig_FOUND -- set false if not found LibConfig
#  LibConfig_RUNTIME_LIBRARY_DIRS -- directory of shared libs *.so (or *.dll files for windows) for program launch
#  LibConfig_INCLUDE_DIRS -- include directory where is can be found LibConfig.h++
#  LibConfig_LIBRARY_DIRS -- directory of libs *.so or *.lib for program linking
#  LibConfig_LIBRARIES -- libraries to link
#  LibConfig_VERSION_STRING -- string containing the version of module
#  LibConfig_VERSION_MAJOR -- major version of module
#  LibConfig_VERSION_MINOR -- minor version of module
#  LibConfig_VERSION_PATCH -- patch version of module
#
#  $LibConfig_ROOT $LibConfig_PATH is an environment variable where LibConfig++ was installed
#

set(LibConfig_FOUND FALSE)

set(LibConfig_INCLUDE_DIRS NOTFOUND)
set(LibConfig_LIBRARIES "")
set(LibConfig_RUNTIME_LIBRARY_DIRS "")
set(LibConfig_LIBRARY_DIRS "")

set(LibConfig_VERSION_STRING NOTFOUND)
set(LibConfig_VERSION_MAJOR NOTFOUND)
set(LibConfig_VERSION_MINOR NOTFOUND)
set(LibConfig_VERSION_PATCH NOTFOUND)

if(CMAKE_FIND_PACKAGE_NAME)
    if(NOT (${CMAKE_FIND_PACKAGE_NAME} STREQUAL LibConfig))
        message(FATAL_ERROR "Wrong package name: ${CMAKE_FIND_PACKAGE_NAME}. Must be: LibConfig.")
    endif(NOT (${CMAKE_FIND_PACKAGE_NAME} STREQUAL LibConfig))
endif(CMAKE_FIND_PACKAGE_NAME)

macro(LibConfig_ERROR_MESSAGE MESSAGE)
    if(LibConfig_FIND_REQUIRED)
        message(FATAL_ERROR ${MESSAGE} ${ARGN})
    elseif(LibConfig_FIND_QUIETLY)
    else(LibConfig_FIND_REQUIRED)
        message(WARNING ${MESSAGE} ${ARGN})
    endif(LibConfig_FIND_REQUIRED)
endmacro(LibConfig_ERROR_MESSAGE)

macro(LibConfig_STATUS_MESSAGE MESSAGE)
    if(NOT LibConfig_FIND_QUIETLY)
        message(STATUS ${MESSAGE} ${ARGN})
    endif(NOT LibConfig_FIND_QUIETLY)
endmacro(LibConfig_STATUS_MESSAGE)

if(UNIX)
    if(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "x86_64")
        set(LibConfig_LIB_PATH_SUFFIXES lib lib/x64 lib/intel64 lib/x86_64-linux-gnu)
        set(LibConfig_BIN_PATH_SUFFIXES lib lib/x64 lib/intel64 lib/x86_64-linux-gnu)
    elseif(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "x86")
        set(LibConfig_LIB_PATH_SUFFIXES lib lib/x86 lib/ia32)
        set(LibConfig_BIN_PATH_SUFFIXES lib lib/x86 lib/ia32)
    endif(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "x86_64")
elseif(WIN32)
    if(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "AMD64")
        set(LibConfig_LIB_PATH_SUFFIXES lib/x64 lib/intel64)
        set(LibConfig_BIN_PATH_SUFFIXES bin/x64 bin/intel64)
    elseif(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "x86")
        set(LibConfig_LIB_PATH_SUFFIXES lib/x86 lib/ia32)
        set(LibConfig_BIN_PATH_SUFFIXES bin/x86 bin/ia32)
    endif(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "AMD64")
endif(UNIX)

macro(LibConfig_FIND_AND_ADD_LIBRARY LIB_NAME LIB_FOUNDED)
    unset(${LIB_NAME}_LIB CACHE)

    find_library(${LIB_NAME}_LIB
      NAMES ${LIB_NAME}
      HINTS
        ENV LibConfig_PATH
        ENV LibConfig_ROOT
      PATH_SUFFIXES ${LibConfig_LIB_PATH_SUFFIXES}
    )

    if(NOT ${LIB_NAME}_LIB)
        LibConfig_ERROR_MESSAGE("Not found ${LIB_NAME} lib.")
    else(NOT ${LIB_NAME}_LIB)
        list(APPEND LibConfig_LIBRARIES ${${LIB_NAME}_LIB})
    endif(NOT ${LIB_NAME}_LIB)

    if(${LIB_NAME}_LIB)
        if(UNIX)
            get_filename_component(LibConfig_RUNTIME_DIR_TEMP ${${LIB_NAME}_LIB} DIRECTORY)
            list(APPEND LibConfig_LIBRARY_DIRS ${LibConfig_RUNTIME_DIR_TEMP})
        elseif(MINGW OR WIN32)
            unset(${LIB_NAME}_DLL CACHE)
            find_file(${LIB_NAME}_DLL
                NAMES ${LIB_NAME}.dll
                HINTS
                    ENV LibConfig_PATH
                    ENV LibConfig_ROOT
                PATH_SUFFIXES ${LibConfig_BIN_PATH_SUFFIXES}
              )
            get_filename_component(LibConfig_RUNTIME_DIR_TEMP ${LIB_NAME}_DLL DIRECTORY)
            list(APPEND LibConfig_LIBRARY_DIRS ${LibConfig_RUNTIME_DIR_TEMP})
            unset(${LIB_NAME}_DLL CACHE)
        endif(UNIX)
        unset(LibConfig_RUNTIME_DIR_TEMP)
    endif(${LIB_NAME}_LIB)

    set(${LIB_FOUNDED} ${LIB_NAME}_LIB)

    unset(${LIB_NAME}_LIB CACHE)
endmacro(LibConfig_FIND_AND_ADD_LIBRARY)

unset(LibConfig_INCLUDE_DIRS_TEMP CACHE)

find_path(LibConfig_INCLUDE_DIRS_TEMP libconfig.h++
  HINTS
    ENV LibConfig_PATH
    ENV LibConfig_ROOT
  PATH_SUFFIXES include
)

if(NOT (EXISTS ${LibConfig_INCLUDE_DIRS_TEMP}))
    LibConfig_ERROR_MESSAGE("Not found LibConfig++ include directory")
else(NOT (EXISTS ${LibConfig_INCLUDE_DIRS_TEMP}))
    set(LibConfig_INCLUDE_DIRS ${LibConfig_INCLUDE_DIRS_TEMP})
endif(NOT (EXISTS ${LibConfig_INCLUDE_DIRS_TEMP}))

unset(LibConfig_INCLUDE_DIRS_TEMP CACHE)

if(EXISTS ${LibConfig_INCLUDE_DIRS})
    set(VERSION_FILE "${LibConfig_INCLUDE_DIRS}/libconfig.h++")
    set(MAJOR_LEVEL_REGEX "^#define[ \t]+LIBCONFIGXX_VER_MAJOR[ \t]+([0-9]+)$")
    set(MINOR_LEVEL_REGEX "^#define[ \t]+LIBCONFIGXX_VER_MINOR[ \t]+([0-9]+)$")
    set(PATCH_LEVEL_REGEX "^#define[ \t]+LIBCONFIGXX_VER_REVISION[ \t]+([0-9]+)$")

    if(EXISTS ${VERSION_FILE})
        file(STRINGS ${VERSION_FILE} VERSION_MAJOR_LINE REGEX ${MAJOR_LEVEL_REGEX})
        file(STRINGS ${VERSION_FILE} VERSION_MINOR_LINE REGEX ${MINOR_LEVEL_REGEX})
        file(STRINGS ${VERSION_FILE} VERSION_PATCH_LINE REGEX ${PATCH_LEVEL_REGEX})
        string(REGEX REPLACE "${MAJOR_LEVEL_REGEX}" "\\1" LibConfig_VERSION_MAJOR "${VERSION_MAJOR_LINE}")
        string(REGEX REPLACE "${MINOR_LEVEL_REGEX}" "\\1" LibConfig_VERSION_MINOR "${VERSION_MINOR_LINE}")
        string(REGEX REPLACE "${PATCH_LEVEL_REGEX}" "\\1" LibConfig_VERSION_PATCH "${VERSION_PATCH_LINE}")
        set(LibConfig_VERSION_STRING ${LibConfig_VERSION_MAJOR}.${LibConfig_VERSION_MINOR}.${LibConfig_VERSION_PATCH})
    endif(EXISTS ${VERSION_FILE})
endif(EXISTS ${LibConfig_INCLUDE_DIRS})

if(LibConfig_FIND_VERSION_EXACT)
    if(LibConfig_FIND_VERSION)
        if(NOT (${LibConfig_VERSION_STRING} VERSION_EQUAL ${LibConfig_FIND_VERSION}))
            if(NOT (${LibConfig_FIND_VERSION_COUNT} EQUAL 3))
                LibConfig_ERROR_MESSAGE("Requested LibConfig version has wrong number of version levels. Must be format [MAJOR].[MINOR].[UPDATE]")
            endif(NOT (${LibConfig_FIND_VERSION_COUNT} EQUAL 3))
            LibConfig_ERROR_MESSAGE("Requested LibConfig ${LibConfig_FIND_VERSION} version. Founded LibConfig ${LibConfig_VERSION_STRING} version.")
        endif(NOT (${LibConfig_VERSION_STRING} VERSION_EQUAL ${LibConfig_FIND_VERSION}))
    endif(LibConfig_FIND_VERSION)
endif(LibConfig_FIND_VERSION_EXACT)

LibConfig_STATUS_MESSAGE("Found LibConfig version: ${LibConfig_VERSION_STRING}")

LibConfig_FIND_AND_ADD_LIBRARY(config++ LibConfig_FOUND)

list(REMOVE_DUPLICATES LibConfig_LIBRARY_DIRS)

