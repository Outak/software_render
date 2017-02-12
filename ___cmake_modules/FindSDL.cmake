#.rst:
# FindSDL
# -------
# - Find SDL library and modules
#
# Use this module by invoking find_package with the form::
#
#   find_package(SDL
#     [version] [EXACT]      # Minimum or EXACT version e.g. 1.2.0
#     [REQUIRED]             # Fail with error if any of SDL modules not found
#     [COMPONENTS <libs>...] # sdl modules by their name
#     )                      #
#
#  possible components -- image, mixer, net, sound, ttf
#
# This module defines
#  SDL_FOUND -- set false if not found SDL
#  SDL_[MODULE NAME]_FOUND -- set false if not found module
#  SDL_RUNTIME_LIBRARY_DIRS -- directory of shared libs *.so (or *.dll files for windows) for program launch
#  SDL_INCLUDE_DIRS -- include directory where is can be found directory SDL/ not file SDL.h
#  SDL_LIBRARY_DIRS -- directory of libs *.so or *.lib for program linking
#  SDL_LIBRARIES -- list of the libraries to link
#  SDL_VERSION_STRING -- string containing the version of module
#  SDL_VERSION_MAJOR -- major version of module
#  SDL_VERSION_MINOR -- minor version of module
#  SDL_VERSION_PATCH -- patch version of module
#  SDL_[MODULE NAME]_VERSION -- string containing the version of module
#
#  $SDLDIR $SDL_DIR an environment variable with path where SDL was installed
#


set(SDL_MODULES_VARIANTS image mixer net sound ttf)
set(SDL_MODULES_VARIANTS_STRING "image, mixer, net, sound, ttf")

set(SDL_FOUND FALSE)
set(SDL_IMAGE_FOUND FALSE)
set(SDL_MIXER_FOUND FALSE)
set(SDL_NET_FOUND FALSE)
set(SDL_SOUND_FOUND FALSE)
set(SDL_TTF_FOUND FALSE)

set(SDL_INCLUDE_DIRS NOTFOUND)
set(SDL_LIBRARIES "")
set(SDL_LIBRARY_DIRS "")
set(SDL_RUNTIME_LIBRARY_DIRS "")

set(SDL_VERSION_STRING NOTFOUND)
set(SDL_VERSION_MAJOR NOTFOUND)
set(SDL_VERSION_MINOR NOTFOUND)
set(SDL_VERSION_PATCH NOTFOUND)
set(SDL_IMAGE_VERSION NOTFOUND)
set(SDL_MIXER_VERSION NOTFOUND)
set(SDL_NET_VERSION NOTFOUND)
set(SDL_SOUND_VERSION NOTFOUND)
set(SDL_TTF_VERSION NOTFOUND)

if(CMAKE_FIND_PACKAGE_NAME)
    if(NOT (${CMAKE_FIND_PACKAGE_NAME} STREQUAL SDL))
        message(FATAL_ERROR "Wrong package name: ${CMAKE_FIND_PACKAGE_NAME}. Must be: SDL.")
    endif(NOT (${CMAKE_FIND_PACKAGE_NAME} STREQUAL SDL))
endif(CMAKE_FIND_PACKAGE_NAME)

macro(SDL_ERROR_MESSAGE MESSAGE)
    if(SDL_FIND_REQUIRED)
        message(FATAL_ERROR ${MESSAGE} ${ARGN})
    elseif(SDL_FIND_QUIETLY)
    else(SDL_FIND_REQUIRED)
        message(WARNING ${MESSAGE} ${ARGN})
    endif(SDL_FIND_REQUIRED)
endmacro(SDL_ERROR_MESSAGE)

macro(SDL_STATUS_MESSAGE MESSAGE)
    if(NOT SDL_FIND_QUIETLY)
        message(STATUS ${MESSAGE} ${ARGN})
    endif(NOT SDL_FIND_QUIETLY)
endmacro(SDL_STATUS_MESSAGE)

if(UNIX)
    if(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "x86_64")
        set(SDL_LIB_PATH_SUFFIXES lib lib/x64 lib/intel64 lib/x86_64-linux-gnu)
        set(SDL_BIN_PATH_SUFFIXES lib lib/x64 lib/intel64 lib/x86_64-linux-gnu)
    elseif(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "x86")
        set(SDL_LIB_PATH_SUFFIXES lib lib/x86 lib/ia32)
        set(SDL_BIN_PATH_SUFFIXES lib lib/x86 lib/ia32)
    endif(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "x86_64")
elseif(WIN32)
    if(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "AMD64")
        set(SDL_LIB_PATH_SUFFIXES lib/x64 lib/intel64)
        set(SDL_BIN_PATH_SUFFIXES bin/x64 bin/intel64)
    elseif(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "x86")
        set(SDL_LIB_PATH_SUFFIXES lib/x86 lib/ia32)
        set(SDL_BIN_PATH_SUFFIXES bin/x86 bin/ia32)
    endif(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "AMD64")
endif(UNIX)

macro(SDL_FIND_AND_ADD_LIBRARY LIB_NAME LIB_FOUNDED)
    unset(${LIB_NAME}_LIB CACHE)

    find_library(${LIB_NAME}_LIB
      NAMES ${LIB_NAME}
      HINTS
        ENV SDLDIR
        ENV SDL_DIR
      PATH_SUFFIXES ${SDL_LIB_PATH_SUFFIXES}
    )

    if(NOT ${LIB_NAME}_LIB)        
        set(${LIB_FOUNDED} FALSE)
        SDL_ERROR_MESSAGE("Not found ${LIB_NAME} lib.")
    else(NOT ${LIB_NAME}_LIB)
        get_filename_component(SDL_DIR_TEMP ${${LIB_NAME}_LIB} DIRECTORY)
        list(APPEND SDL_LIBRARY_DIRS ${SDL_DIR_TEMP})
        unset(SDL_DIR_TEMP)
        list(APPEND SDL_LIBRARIES ${${LIB_NAME}_LIB})
        set(${LIB_FOUNDED} TRUE)
    endif(NOT ${LIB_NAME}_LIB)

    if(${LIB_NAME}_LIB)
        if(UNIX)
            get_filename_component(SDL_RUNTIME_DIR_TEMP ${${LIB_NAME}_LIB} DIRECTORY)
        elseif(MINGW OR WIN32)
            unset(${LIB_NAME}_DLL CACHE)
            find_file(${LIB_NAME}_DLL
                NAMES ${LIB_NAME}.dll
                HINTS
                  ENV SDLDIR
                  ENV SDL_DIR
                PATH_SUFFIXES ${SDL_BIN_PATH_SUFFIXES}
              )
            get_filename_component(SDL_RUNTIME_DIR_TEMP ${LIB_NAME}_DLL DIRECTORY)
            unset(${LIB_NAME}_DLL CACHE)
        endif(UNIX)
        list(APPEND SDL_RUNTIME_LIBRARY_DIRS ${SDL_RUNTIME_DIR_TEMP})
        unset(SDL_RUNTIME_DIR_TEMP)
    endif(${LIB_NAME}_LIB)

    unset(${LIB_NAME}_LIB CACHE)
endmacro(SDL_FIND_AND_ADD_LIBRARY)

unset(SDL_INCLUDE_DIRS_TEMP CACHE)

find_path(SDL_INCLUDE_DIRS_TEMP SDL.h
  HINTS
    ENV SDLDIR
    ENV SDL_DIR
  PATH_SUFFIXES include/SDL
)

if(NOT (EXISTS ${SDL_INCLUDE_DIRS_TEMP}))
    SDL_ERROR_MESSAGE("Not found sdl include directory.")
else(NOT (EXISTS ${SDL_INCLUDE_DIRS_TEMP}))
    get_filename_component(SDL_INCLUDE_DIRS_TEMP ${SDL_INCLUDE_DIRS_TEMP} DIRECTORY)
    set(SDL_INCLUDE_DIRS ${SDL_INCLUDE_DIRS_TEMP})
endif(NOT (EXISTS ${SDL_INCLUDE_DIRS_TEMP}))

unset(SDL_INCLUDE_DIRS_TEMP CACHE)

if(EXISTS ${SDL_INCLUDE_DIRS})
    set(VERSION_FILE "${SDL_INCLUDE_DIRS}/SDL/SDL_version.h")
    set(MAJOR_LEVEL_REGEX "^#define[ \t]+SDL_MAJOR_VERSION[ \t]+([0-9]+)$")
    set(MINOR_LEVEL_REGEX "^#define[ \t]+SDL_MINOR_VERSION[ \t]+([0-9]+)$")
    set(PATCH_LEVEL_REGEX "^#define[ \t]+SDL_PATCHLEVEL[ \t]+([0-9]+)$")

    if(EXISTS ${VERSION_FILE})
        file(STRINGS ${VERSION_FILE} VERSION_MAJOR_LINE REGEX ${MAJOR_LEVEL_REGEX})
        file(STRINGS ${VERSION_FILE} VERSION_MINOR_LINE REGEX ${MINOR_LEVEL_REGEX})
        file(STRINGS ${VERSION_FILE} VERSION_PATCH_LINE REGEX ${PATCH_LEVEL_REGEX})
        string(REGEX REPLACE "${MAJOR_LEVEL_REGEX}" "\\1" SDL_VERSION_MAJOR "${VERSION_MAJOR_LINE}")
        string(REGEX REPLACE "${MINOR_LEVEL_REGEX}" "\\1" SDL_VERSION_MINOR "${VERSION_MINOR_LINE}")
        string(REGEX REPLACE "${PATCH_LEVEL_REGEX}" "\\1" SDL_VERSION_PATCH "${VERSION_PATCH_LINE}")
        set(SDL_VERSION_STRING ${SDL_VERSION_MAJOR}.${SDL_VERSION_MINOR}.${SDL_VERSION_PATCH})
    endif(EXISTS ${VERSION_FILE})
endif(EXISTS ${SDL_INCLUDE_DIRS})

if(MINGW OR WIN32)
    SDL_FIND_AND_ADD_LIBRARY(SDL SDL_LIB SDL_MAIN_FOUND)
endif(MINGW OR WIN32)

SDL_FIND_AND_ADD_LIBRARY(SDL SDL_FOUND)

if(SDL_FIND_VERSION_EXACT)
    if(SDL_FIND_VERSION)
        if(NOT (${SDL_VERSION_STRING} VERSION_EQUAL ${SDL_FIND_VERSION}))
            if(NOT (${SDL_FIND_VERSION_COUNT} EQUAL 3))
                SDL_ERROR_MESSAGE("Requested SDL version has wrong number of version levels. Must be format [MAJOR].[MINOR].[UPDATE]")
            endif(NOT (${SDL_FIND_VERSION_COUNT} EQUAL 3))
            SDL_ERROR_MESSAGE("Requested SDL ${SDL_FIND_VERSION} version. Founded SDL ${SDL_VERSION_STRING} version.")
        endif(NOT (${SDL_VERSION_STRING} VERSION_EQUAL ${SDL_FIND_VERSION}))
    endif(SDL_FIND_VERSION)
endif(SDL_FIND_VERSION_EXACT)

SDL_STATUS_MESSAGE("Found SDL version: ${SDL_VERSION_STRING}")

macro(SDL_GET_MODULE_VERSION MODULE_NAME)
    if(EXISTS ${SDL_INCLUDE_DIRS})
        string(TOUPPER ${MODULE_NAME} SDL_MODULE_TEMP_STRING)

        set(VERSION_FILE "${SDL2_INCLUDE_DIR}/SDL/SDL_${MODULE_NAME}.h")
        set(MAJOR_LEVEL_REGEX "^#define[ \t]+SDL_${SDL_MODULE_TEMP_STRING}_MAJOR_VERSION[ \t]+([0-9]+)$")
        set(MINOR_LEVEL_REGEX "^#define[ \t]+SDL_${SDL_MODULE_TEMP_STRING}_MINOR_VERSION[ \t]+([0-9]+)$")
        set(PATCH_LEVEL_REGEX "^#define[ \t]+SDL_${SDL_MODULE_TEMP_STRING}_PATCHLEVEL[ \t]+([0-9]+)$")

        if(EXISTS ${VERSION_FILE})
            file(STRINGS ${VERSION_FILE} VERSION_MAJOR_LINE REGEX ${MAJOR_LEVEL_REGEX})
            file(STRINGS ${VERSION_FILE} VERSION_MINOR_LINE REGEX ${MINOR_LEVEL_REGEX})
            file(STRINGS ${VERSION_FILE} VERSION_PATCH_LINE REGEX ${PATCH_LEVEL_REGEX})
            string(REGEX REPLACE "${MAJOR_LEVEL_REGEX}" "\\1" VERSION_MAJOR "${VERSION_MAJOR_LINE}")
            string(REGEX REPLACE "${MINOR_LEVEL_REGEX}" "\\1" VERSION_MINOR "${VERSION_MINOR_LINE}")
            string(REGEX REPLACE "${PATCH_LEVEL_REGEX}" "\\1" VERSION_PATCH "${VERSION_PATCH_LINE}")
                set(SDL_${SDL_MODULE_TEMP_STRING}_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
                SDL_STATUS_MESSAGE("Found SDL module ${MODULE_NAME} version: ${SDL_${SDL_MODULE_TEMP_STRING}_VERSION}")
        endif(EXISTS ${VERSION_FILE})

        unset(SDL_MODULE_TEMP_STRING)
    endif(EXISTS ${SDL_INCLUDE_DIRS})
endmacro(SDL_GET_MODULE_VERSION)

macro(SDL_FIND_MODULE MODULE_NAME)
    set(SDL_MODULE_LIB_NAME SDL_${MODULE_NAME})
    string(TOUPPER ${MODULE_NAME} SDL_MODULE_TEMP_STRING)
    SDL_FIND_AND_ADD_LIBRARY(${SDL_MODULE_LIB_NAME} SDL_${MODULE_NAME}_FOUND)
endmacro(SDL_FIND_MODULE)

foreach(SDL_CUR_COMPONENT ${SDL_FIND_COMPONENTS})
    list(FIND SDL_MODULES_VARIANTS ${SDL_CUR_COMPONENT} index)
    if(${index} EQUAL -1)
        SDL_ERROR_MESSAGE("Not acceptable component name \"${SDL_CUR_COMPONENT}\". Must be one of: ${SDL_MODULES_VARIANTS_STRING}.")
        unset(FINE_TMP_LIST)
    else(${index} EQUAL -1)
        SDL_FIND_MODULE(${SDL_CUR_COMPONENT})
        SDL_GET_MODULE_VERSION(${SDL_CUR_COMPONENT})
    endif(${index} EQUAL -1)
    unset(index)
endforeach(SDL_CUR_COMPONENT)

list(REMOVE_DUPLICATES SDL_LIBRARY_DIRS)
list(REMOVE_DUPLICATES SDL_RUNTIME_LIBRARY_DIRS)

unset(SDL_MODULES_VARIANTS)
