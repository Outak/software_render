cmake_minimum_required(VERSION 2.8)

# - Find SDL2 library and modules
# This module defines
#  SDL2_FOUND -- set false if not found SDL
#  SDL2_[MODULE NAME]_FOUND -- set false if not found module
#  SDL2_INCLUDE_DIR -- include directory where is can be found directory SDL2/ not file SDL.h
#  SDL2_LIBRARIES -- list of the libraries to link
#  SDL2_RUNTIME_LIBRARY_DIRS -- directory of shared libs *.so (or *.dll files for windows) for program launch
#  SDL2_VERSION_STRING -- string containing the version of module
#  SDL2_VERSION_MAJOR -- major version of module
#  SDL2_VERSION_MINOR -- minor version of module
#  SDL2_VERSION_PATCH -- patch version of module
#  SDL2_[MODULE NAME]_VERSION -- string containing the version of module
#
#  possible components -- image, mixer, net, gfx, ttf
#
#  $SDL2DIR $SDL_DIR $SDL_PATH is an environment variable where SDL2 was installed
#

set(SDL2_FOUND FALSE)
set(SDL2_IMAGE_FOUND FALSE)
set(SDL2_MIXER_FOUND FALSE)
set(SDL2_NET_FOUND FALSE)
set(SDL2_GFX_FOUND FALSE)
set(SDL2_TTF_FOUND FALSE)

set(SDL2_INCLUDE_DIR NOTFOUND)
set(SDL2_LIBRARIES "")
set(SDL2_RUNTIME_LIBRARY_DIRS "")

set(SDL2_VERSION_STRING NOTFOUND)
set(SDL2_VERSION_MAJOR NOTFOUND)
set(SDL2_VERSION_MINOR NOTFOUND)
set(SDL2_VERSION_PATCH NOTFOUND)
set(SDL2_IMAGE_VERSION NOTFOUND)
set(SDL2_MIXER_VERSION NOTFOUND)
set(SDL2_NET_VERSION NOTFOUND)
set(SDL2_GFX_VERSION NOTFOUND)
set(SDL2_TTF_VERSION NOTFOUND)

set(SDL2_MODULES_VARIANTS image mixer net gfx ttf)

if(CMAKE_FIND_PACKAGE_NAME)
    if(NOT (${CMAKE_FIND_PACKAGE_NAME} STREQUAL SDL2))
        message(FATAL_ERROR "Wrong package name: ${CMAKE_FIND_PACKAGE_NAME}. Must be: SDL2.")
    endif(NOT (${CMAKE_FIND_PACKAGE_NAME} STREQUAL SDL2))
endif(CMAKE_FIND_PACKAGE_NAME)

macro(SDL2_ERROR_MESSAGE MESSAGE)
    if(SDL2_FIND_REQUIRED)
        message(FATAL_ERROR ${MESSAGE} ${ARGN})
    elseif(SDL2_FIND_QUIETLY)
    else(SDL2_FIND_REQUIRED)
        message(WARNING ${MESSAGE} ${ARGN})
    endif(SDL2_FIND_REQUIRED)
endmacro(SDL2_ERROR_MESSAGE)

macro(SDL2_STATUS_MESSAGE MESSAGE)
    if(NOT SDL2_FIND_QUIETLY)
        message(STATUS ${MESSAGE} ${ARGN})
    endif(NOT SDL2_FIND_QUIETLY)
endmacro(SDL2_STATUS_MESSAGE)

function(FINE_LIST_PRINT DST DELIMITER)
    list(GET ARGV 2 temp)
    math(EXPR N "${ARGC}-1")
    foreach(IDX RANGE 3 ${N})
        list(GET ARGV ${IDX} STR)
        set(temp "${temp}${DELIMITER}${STR}")
    endforeach()
    set(${DST} "${temp}" PARENT_SCOPE)
endfunction(FINE_LIST_PRINT DST DELIMITER)

macro(SDL2_FIND_AND_ADD_LIBRARY LIB_NAME LIB_FOUNDED)
    unset(${LIB_NAME}_LIB CACHE)

    find_library(${LIB_NAME}_LIB
      NAMES ${LIB_NAME}
      HINTS
        ENV SDLDIR
        ENV SDL_DIR
        ENV SDL_PATH
      PATH_SUFFIXES lib/x86_64-linux-gnu/ lib/x64
    )

    if(NOT ${LIB_NAME}_LIB)
        SDL2_ERROR_MESSAGE("Not found ${LIB_NAME} lib.")
    else(NOT ${LIB_NAME}_LIB)
        list(APPEND SDL2_LIBRARIES ${${LIB_NAME}_LIB})
    endif(NOT ${LIB_NAME}_LIB)

    if(${LIB_NAME}_LIB)
        if(UNIX)
            get_filename_component(SDL2_RUNTIME_DIR_TEMP ${${LIB_NAME}_LIB} DIRECTORY)
            list(APPEND SDL2_RUNTIME_LIBRARY_DIRS ${SDL2_RUNTIME_DIR_TEMP})
        elseif(MINGW OR WIN32)
            unset(${LIB_NAME}_DLL CACHE)
            find_file(${LIB_NAME}_DLL
                NAMES ${LIB_NAME}.dll
                HINTS
                  ENV SDLDIR
                  ENV SDL_DIR
                  ENV SDL_PATH
                PATH_SUFFIXES lib lib/x86_64-linux-gnu/ lib/x64 bin
              )
            get_filename_component(SDL2_RUNTIME_DIR_TEMP ${LIB_NAME}_DLL DIRECTORY)
            list(APPEND SDL2_RUNTIME_LIBRARY_DIRS ${SDL2_RUNTIME_DIR_TEMP})
            unset(${LIB_NAME}_DLL CACHE)
        endif(UNIX)
        unset(SDL2_RUNTIME_DIR_TEMP)
    endif(${LIB_NAME}_LIB)

    set(${LIB_FOUNDED} ${LIB_NAME}_LIB)

    unset(${LIB_NAME}_LIB CACHE)
endmacro(SDL2_FIND_AND_ADD_LIBRARY)

unset(SDL2_INCLUDE_DIR_TEMP CACHE)

find_path(SDL2_INCLUDE_DIR_TEMP SDL.h
  HINTS
    /usr
    ENV SDLDIR
    ENV SDL_DIR
    ENV SDL_PATH
  PATH_SUFFIXES include/SDL2
)

if(NOT (EXISTS ${SDL2_INCLUDE_DIR_TEMP}))
    SDL2_ERROR_MESSAGE("Not found sdl2 include directory")
else(NOT (EXISTS ${SDL2_INCLUDE_DIR_TEMP}))
    get_filename_component(SDL2_INCLUDE_DIR_TEMP ${SDL2_INCLUDE_DIR_TEMP} DIRECTORY)
    set(SDL2_INCLUDE_DIR ${SDL2_INCLUDE_DIR_TEMP})
endif(NOT (EXISTS ${SDL2_INCLUDE_DIR_TEMP}))

unset(SDL2_INCLUDE_DIR_TEMP CACHE)

if(EXISTS ${SDL2_INCLUDE_DIR})
    set(VERSION_FILE "${SDL2_INCLUDE_DIR}/SDL2/SDL_version.h")
    set(MAJOR_LEVEL_REGEX "^#define[ \t]+SDL_MAJOR_VERSION[ \t]+([0-9]+)$")
    set(MINOR_LEVEL_REGEX "^#define[ \t]+SDL_MINOR_VERSION[ \t]+([0-9]+)$")
    set(PATCH_LEVEL_REGEX "^#define[ \t]+SDL_PATCHLEVEL[ \t]+([0-9]+)$")

    if(EXISTS ${VERSION_FILE})
        file(STRINGS ${VERSION_FILE} VERSION_MAJOR_LINE REGEX ${MAJOR_LEVEL_REGEX})
        file(STRINGS ${VERSION_FILE} VERSION_MINOR_LINE REGEX ${MINOR_LEVEL_REGEX})
        file(STRINGS ${VERSION_FILE} VERSION_PATCH_LINE REGEX ${PATCH_LEVEL_REGEX})
        string(REGEX REPLACE "${MAJOR_LEVEL_REGEX}" "\\1" SDL2_VERSION_MAJOR "${VERSION_MAJOR_LINE}")
        string(REGEX REPLACE "${MINOR_LEVEL_REGEX}" "\\1" SDL2_VERSION_MINOR "${VERSION_MINOR_LINE}")
        string(REGEX REPLACE "${PATCH_LEVEL_REGEX}" "\\1" SDL2_VERSION_PATCH "${VERSION_PATCH_LINE}")
        set(SDL2_VERSION_STRING ${SDL2_VERSION_MAJOR}.${SDL2_VERSION_MINOR}.${SDL2_VERSION_PATCH})
    endif(EXISTS ${VERSION_FILE})
endif(EXISTS ${SDL2_INCLUDE_DIR})

if(MINGW OR WIN32)
    SDL2_FIND_AND_ADD_LIBRARY(SDL2main SDL2_MAIN_LIB)
endif(MINGW OR WIN32)

SDL2_FIND_AND_ADD_LIBRARY(SDL2 SDL2_LIB)

if(SDL2_LIB)
    if(MINGW OR WIN32)
        if(SDL2_MAIN_LIB)
            set(SDL2_FOUND TRUE)
        endif(SDL2_MAIN_LIB)
    else(MINGW OR WIN32)
        set(SDL2_FOUND TRUE)
    endif(MINGW OR WIN32)
endif(SDL2_LIB)

unset(SDL2_SDL2_LIB CACHE)

SDL2_STATUS_MESSAGE("Found SDL2 version: ${SDL2_VERSION_STRING}")

macro(SDL2_GET_MODULE_VERSION MODULE_NAME)
    if(EXISTS ${SDL2_INCLUDE_DIR})
        string(TOUPPER ${MODULE_NAME} SDL2_MODULE_TEMP_STRING)
        if(${MODULE_NAME} STREQUAL "gfx")
            set(VERSION_FILE "${SDL2_INCLUDE_DIR}/SDL2/SDL_gfxPrimitives.h")
            set(MAJOR_LEVEL_REGEX "^#define[ \t]+SDL2_GFXPRIMITIVES_MAJOR[ \t]+([0-9]+)$")
            set(MINOR_LEVEL_REGEX "^#define[ \t]+SDL2_GFXPRIMITIVES_MINOR[ \t]+([0-9]+)$")
            set(PATCH_LEVEL_REGEX "^#define[ \t]+SDL2_GFXPRIMITIVES_MICRO[ \t]+([0-9]+)$")
        else(${MODULE_NAME} STREQUAL "gfx")
            set(VERSION_FILE "${SDL2_INCLUDE_DIR}/SDL2/SDL_${MODULE_NAME}.h")
            set(MAJOR_LEVEL_REGEX "^#define[ \t]+SDL_${SDL2_MODULE_TEMP_STRING}_MAJOR_VERSION[ \t]+([0-9]+)$")
            set(MINOR_LEVEL_REGEX "^#define[ \t]+SDL_${SDL2_MODULE_TEMP_STRING}_MINOR_VERSION[ \t]+([0-9]+)$")
            set(PATCH_LEVEL_REGEX "^#define[ \t]+SDL_${SDL2_MODULE_TEMP_STRING}_PATCHLEVEL[ \t]+([0-9]+)$")
        endif(${MODULE_NAME} STREQUAL "gfx")

        if(EXISTS ${VERSION_FILE})
            file(STRINGS ${VERSION_FILE} VERSION_MAJOR_LINE REGEX ${MAJOR_LEVEL_REGEX})
            file(STRINGS ${VERSION_FILE} VERSION_MINOR_LINE REGEX ${MINOR_LEVEL_REGEX})
            file(STRINGS ${VERSION_FILE} VERSION_PATCH_LINE REGEX ${PATCH_LEVEL_REGEX})
            string(REGEX REPLACE "${MAJOR_LEVEL_REGEX}" "\\1" VERSION_MAJOR "${VERSION_MAJOR_LINE}")
            string(REGEX REPLACE "${MINOR_LEVEL_REGEX}" "\\1" VERSION_MINOR "${VERSION_MINOR_LINE}")
            string(REGEX REPLACE "${PATCH_LEVEL_REGEX}" "\\1" VERSION_PATCH "${VERSION_PATCH_LINE}")
                set(SDL2_${SDL2_MODULE_TEMP_STRING}_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
                SDL2_STATUS_MESSAGE("Found SDL2 module ${MODULE_NAME} version: ${SDL2_${SDL2_MODULE_TEMP_STRING}_VERSION}")
        endif(EXISTS ${VERSION_FILE})

        unset(SDL2_MODULE_TEMP_STRING)
    endif(EXISTS ${SDL2_INCLUDE_DIR})
endmacro(SDL2_GET_MODULE_VERSION)

macro(SDL2_FIND_MODULE MODULE_NAME)
    set(SDL2_MODULE_LIB_NAME SDL2_${MODULE_NAME})
    string(TOUPPER ${MODULE_NAME} SDL2_MODULE_TEMP_STRING)
    SDL2_FIND_AND_ADD_LIBRARY(${SDL2_MODULE_LIB_NAME} SDL2_${MODULE_NAME}_LIB)
endmacro(SDL2_FIND_MODULE)

foreach(SDL2_CUR_COMPONENT ${SDL2_FIND_COMPONENTS})
    list(FIND SDL2_MODULES_VARIANTS ${SDL2_CUR_COMPONENT} index)
    if(${index} EQUAL -1)
        FINE_LIST_PRINT(FINE_TMP_LIST ", " ${IPP_MODULES_VARIANTS})
        SDL2_ERROR_MESSAGE("Not acceptable component name \"${SDL2_CUR_COMPONENT}\". Must be one of: ${FINE_TMP_LIST}.")
        unset(FINE_TMP_LIST)
    else(${index} EQUAL -1)
        SDL2_FIND_MODULE(${SDL2_CUR_COMPONENT})
        SDL2_GET_MODULE_VERSION(${SDL2_CUR_COMPONENT})
    endif(${index} EQUAL -1)
    unset(index)
endforeach(SDL2_CUR_COMPONENT)

list(REMOVE_DUPLICATES SDL2_RUNTIME_LIBRARY_DIRS)

unset(SDL2_MODULES_VARIANTS)
