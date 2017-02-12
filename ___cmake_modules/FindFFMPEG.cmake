cmake_minimum_required(VERSION 2.8)

# - Find FFMPEG library and modules
# This module defines
#  FFMPEG_FOUND -- set false if not found SDL
#  FFMPEG_[MODULE NAME]_FOUND -- set false if not found module
#  FFMPEG_INCLUDE_DIR -- include directory where is can be found directory libavcodec
#  FFMPEG_LIBRARIES -- list of the libraries to link
#  FFMPEG_DEFINITIONS -- list of defenitions
#  FFMPEG_RUNTIME_LIBRARY_DIRS -- directory of shared libs *.so (or *.dll files for windows) for program launch
#  FFMPEG_VERSION_STRING -- string containing the version of module
#  FFMPEG_VERSION_MAJOR -- major version of module
#  FFMPEG_VERSION_MINOR -- minor version of module
#  FFMPEG_VERSION_PATCH -- patch version of module
#  FFMPEG_[MODULE NAME]_VERSION -- string containing the version of module
#
#  possible components -- avcodec, avformat, avdevice, avutil, swscale, swresample
#
#  $FFMPEG_DIR $FFMPEG_PATH is an environment variable where FFMPEG was installed
#

set(FFMPEG_FOUND FALSE)
set(FFMPEG_AVCODEC_FOUND FALSE)
set(FFMPEG_AVDEVICE_FOUND FALSE)
set(FFMPEG_AVFORMAT_FOUND FALSE)
set(FFMPEG_AVUTIL_FOUND FALSE)
set(FFMPEG_SWSCALE_FOUND FALSE)
set(FFMPEG_SWRESAMPLE_FOUND FALSE)

set(FFMPEG_INCLUDE_DIR NOTFOUND)
set(FFMPEG_LIBRARIES "")
set(FFMPEG_DEFINITIONS "")
set(FFMPEG_RUNTIME_LIBRARY_DIRS "")

set(FFMPEG_VERSION_STRING NOTFOUND)
set(FFMPEG_VERSION_MAJOR NOTFOUND)
set(FFMPEG_VERSION_MINOR NOTFOUND)
set(FFMPEG_VERSION_MICRO NOTFOUND)
set(FFMPEG_AVCODEC_VERSION_STRING NOTFOUND)
set(FFMPEG_AVDEVICE_VERSION_STRING NOTFOUND)
set(FFMPEG_AVFORMAT_VERSION_STRING NOTFOUND)
set(FFMPEG_AVUTIL_VERSION_STRING NOTFOUND)
set(FFMPEG_SWSCALE_VERSION_STRING NOTFOUND)
set(FFMPEG_SWRESAMPLE_VERSION_STRING NOTFOUND)

set(FFMPEG_MODULES_VARIANTS avcodec avformat avdevice avutil swscale swresample)

if(${CMAKE_FIND_PACKAGE_NAME})
    if(NOT (${CMAKE_FIND_PACKAGE_NAME} STREQUAL FFMPEG))
        message(FATAL_ERROR "Wrong package name: ${CMAKE_FIND_PACKAGE_NAME}. Must be: FFMPEG.")
    endif(NOT (${CMAKE_FIND_PACKAGE_NAME} STREQUAL FFMPEG))
endif(${CMAKE_FIND_PACKAGE_NAME})

macro(FFMPEG_ERROR_MESSAGE MESSAGE)
    if(FFMPEG_FIND_REQUIRED)
        message(FATAL_ERROR ${MESSAGE} ${ARGN})
    elseif(NOT FFMPEG_FIND_QUIETLY)
        message(WARNING ${MESSAGE} ${ARGN})
    endif(FFMPEG_FIND_REQUIRED)
endmacro(FFMPEG_ERROR_MESSAGE)

macro(FFMPEG_STATUS_MESSAGE MESSAGE)
    if(NOT FFMPEG_FIND_QUIETLY)
        message(STATUS ${MESSAGE} ${ARGN})
    endif(NOT FFMPEG_FIND_QUIETLY)
endmacro(FFMPEG_STATUS_MESSAGE)

function(FINE_LIST_PRINT DST DELIMITER)
    list(GET ARGV 2 temp)
    math(EXPR N "${ARGC}-1")
    foreach(IDX RANGE 3 ${N})
        list(GET ARGV ${IDX} STR)
        set(temp "${temp}${DELIMITER}${STR}")
    endforeach()
    set(${DST} "${temp}" PARENT_SCOPE)
endfunction(FINE_LIST_PRINT DST DELIMITER)

macro(FFMPEG_FIND_AND_ADD_LIBRARY LIB_NAME LIB_FOUNDED)
    unset(${LIB_NAME}_LIB CACHE)

    find_library(${LIB_NAME}_LIB
      NAMES ${LIB_NAME}
      HINTS
        ENV FFMPEG_DIR
        ENV FFMPEG_PATH
      PATH_SUFFIXES lib/x86_64-linux-gnu/ lib/x64
    )

    if(NOT ${LIB_NAME}_LIB)
        FFMPEG_ERROR_MESSAGE("Not found ${LIB_NAME} lib.")
    else(NOT ${LIB_NAME}_LIB)
        list(APPEND FFMPEG_LIBRARIES ${${LIB_NAME}_LIB})
    endif(NOT ${LIB_NAME}_LIB)

    if(${LIB_NAME}_LIB)
        if(UNIX)
            get_filename_component(FFMPEG_RUNTIME_DIR_TEMP ${${LIB_NAME}_LIB} DIRECTORY)
            list(APPEND FFMPEG_RUNTIME_LIBRARY_DIRS ${FFMPEG_RUNTIME_DIR_TEMP})
        elseif(MINGW OR WIN32)
            unset(${LIB_NAME}_DLL CACHE)
            find_file(${LIB_NAME}_DLL
                NAMES ${LIB_NAME}.dll
                HINTS
                  ENV FFMPEG_DIR
                  ENV FFMPEG_PATH
                PATH_SUFFIXES lib lib/x86_64-linux-gnu/ lib/x64 bin
              )
            get_filename_component(FFMPEG_RUNTIME_DIR_TEMP ${LIB_NAME}_DLL DIRECTORY)
            list(APPEND FFMPEG_RUNTIME_LIBRARY_DIRS ${FFMPEG_RUNTIME_DIR_TEMP})
            unset(${LIB_NAME}_DLL CACHE)
        endif(UNIX)
        unset(FFMPEG_RUNTIME_DIR_TEMP)
    endif(${LIB_NAME}_LIB)

    set(${LIB_FOUNDED} ${LIB_NAME}_LIB)

    unset(${LIB_NAME}_LIB CACHE)
endmacro(FFMPEG_FIND_AND_ADD_LIBRARY)

unset(FFMPEG_INCLUDE_DIR_TEMP CACHE)

find_path(FFMPEG_INCLUDE_DIR_TEMP libavcodec
  HINTS
    ENV FFMPEG_DIR
    ENV FFMPEG_PATH
  PATH_SUFFIXES include
)

if(NOT (EXISTS ${FFMPEG_INCLUDE_DIR_TEMP}))
    FFMPEG_ERROR_MESSAGE("Not found ffmpeg include directory")
else(NOT (EXISTS ${FFMPEG_INCLUDE_DIR_TEMP}))
    set(FFMPEG_INCLUDE_DIR ${FFMPEG_INCLUDE_DIR_TEMP})
endif(NOT (EXISTS ${FFMPEG_INCLUDE_DIR_TEMP}))

unset(FFMPEG_INCLUDE_DIR_TEMP CACHE)

macro(FFMPEG_GET_MODULE_VERSION MODULE_NAME)
    if(EXISTS ${FFMPEG_INCLUDE_DIR})
        string(TOUPPER ${MODULE_NAME} FFMPEG_MODULE_TEMP_STRING)

        set(VERSION_FILE "${FFMPEG_INCLUDE_DIR}/lib${MODULE_NAME}/version.h")
        set(MAJOR_REGEX "^#define[ \t]+LIB${FFMPEG_MODULE_TEMP_STRING}_VERSION_MAJOR[ \t]+([0-9]+)$")
        set(MINOR_REGEX "^#define[ \t]+LIB${FFMPEG_MODULE_TEMP_STRING}_VERSION_MINOR[ \t]+([0-9]+)$")
        set(MICRO_REGEX "^#define[ \t]+LIB${FFMPEG_MODULE_TEMP_STRING}_VERSION_MICRO[ \t]+([0-9]+)$")

        if(EXISTS ${VERSION_FILE})
            file(STRINGS ${VERSION_FILE} VERSION_MAJOR_LINE REGEX ${MAJOR_REGEX})
            file(STRINGS ${VERSION_FILE} VERSION_MINOR_LINE REGEX ${MINOR_REGEX})
            file(STRINGS ${VERSION_FILE} VERSION_MICRO_LINE REGEX ${MICRO_REGEX})
            string(REGEX REPLACE "${MAJOR_REGEX}" "\\1" VERSION_MAJOR "${VERSION_MAJOR_LINE}")
            string(REGEX REPLACE "${MINOR_REGEX}" "\\1" VERSION_MINOR "${VERSION_MINOR_LINE}")
            string(REGEX REPLACE "${MICRO_REGEX}" "\\1" VERSION_MICRO "${VERSION_MICRO_LINE}")
                set(FFMPEG_${FFMPEG_MODULE_TEMP_STRING}_VERSION_STRING ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_MICRO})
                FFMPEG_STATUS_MESSAGE("Found FFMPEG module ${MODULE_NAME} version: ${FFMPEG_${FFMPEG_MODULE_TEMP_STRING}_VERSION_STRING}")
            if(${MODULE_NAME} STREQUAL avcodec)
                set(FFMPEG_VERSION_MAJOR ${VERSION_MAJOR})
                set(FFMPEG_VERSION_MINOR ${VERSION_MINOR})
                set(FFMPEG_VERSION_MICRO ${VERSION_MICRO})
            endif(${MODULE_NAME} STREQUAL avcodec)
        endif(EXISTS ${VERSION_FILE})

        unset(FFMPEG_MODULE_TEMP_STRING)
    endif(EXISTS ${FFMPEG_INCLUDE_DIR})
endmacro(FFMPEG_GET_MODULE_VERSION)

macro(FFMPEG_FIND_MODULE MODULE_NAME)
    set(FFMPEG_MODULE_LIB_NAME ${MODULE_NAME})
    string(TOUPPER ${MODULE_NAME} FFMPEG_MODULE_TEMP_STRING)
    FFMPEG_FIND_AND_ADD_LIBRARY(${FFMPEG_MODULE_LIB_NAME} FFMPEG_MODULE_LIB_TEMP)
endmacro(FFMPEG_FIND_MODULE)

if(NOT FFMPEG_FIND_COMPONENTS)
    FFMPEG_ERROR_MESSAGE("Not found any module lib.")
endif(NOT FFMPEG_FIND_COMPONENTS)

foreach(FFMPEG_CUR_COMPONENT ${FFMPEG_FIND_COMPONENTS})
    list(FIND FFMPEG_MODULES_VARIANTS ${FFMPEG_CUR_COMPONENT} index)
    if(${index} EQUAL -1)
        FINE_LIST_PRINT(FINE_TMP_LIST ", " ${IPP_MODULES_VARIANTS})
        FFMPEG_ERROR_MESSAGE("Not acceptable component name \"${FFMPEG_CUR_COMPONENT}\". Must be one of: ${FINE_TMP_LIST}.")
        unset(FINE_TMP_LIST)
    else(${index} EQUAL -1)
        FFMPEG_FIND_MODULE(${FFMPEG_CUR_COMPONENT})
        FFMPEG_GET_MODULE_VERSION(${FFMPEG_CUR_COMPONENT})
    endif(${index} EQUAL -1)
    unset(index)
endforeach(FFMPEG_CUR_COMPONENT)

list(REMOVE_DUPLICATES FFMPEG_RUNTIME_LIBRARY_DIRS)

unset(FFMPEG_MODULES_VARIANTS)
