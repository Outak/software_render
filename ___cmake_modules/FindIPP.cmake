#.rst:
# FindIpp
# -------
# - Find IPP library and include directiries
#
# Use this module by invoking find_package with the form::
#
#   find_package(IPP
#     [version] [EXACT]      # Minimum or EXACT version e.g. 6.0.0
#     [REQUIRED]             # Fail with error if any of IPP modules not found
#     [COMPONENTS <libs>...] # ipp modules by their name
#     )                      #
#
#  possible components -- core, signal_processing, image_processing, color_conversion, computer_vision, vector_math
#
# This module defines
#  IPP_FOUND -- set false if not found IPP
#  IPP_RUNTIME_LIBRARY_DIRS -- directory of shared libs *.so (or *.dll files for windows) for program launch
#  IPP_INCLUDE_DIRS -- include directory where is can be found directory ipp.h
#  IPP_LIBRARY_DIRS -- directory of libs *.so or *.lib for program linking
#  IPP_LIBRARIES -- list of the libraries to link
#  IPP_VERSION_STRING -- string containing the version of IPP
#  IPP_VERSION_MAJOR -- major version of IPP
#  IPP_VERSION_MINOR -- minor version of IPP
#  IPP_VERSION_PATCH -- patch version of IPP
#
#  $IPPPATH $IPP_PATH is an environment variable with path where IPP was installed
#


if(UNIX)
    set(IPP_STD_PATH "/opt/intel/ipp")
elseif(WIN32)
    set(IPP_STD_PATH "C:/Programm Files/Intel/ipp")
endif(UNIX)

if(NOT (EXISTS ${IPP_STD_PATH}))
    if(EXIST $ENV{IPPPATH})
        set(IPP_PATH $ENV{IPPPATH})
    elseif(EXISTS $ENV{IPP_PATH})
        set(IPP_PATH $ENV{IPP_PATH})
    endif(EXIST $ENV{IPPPATH})
else(NOT (EXISTS ${IPP_STD_PATH}))
    if(IS_DIRECTORY ${IPP_STD_PATH})
        set(IPP_PATH ${IPP_STD_PATH})
    endif(IS_DIRECTORY ${IPP_STD_PATH})
endif(NOT (EXISTS ${IPP_STD_PATH}))

set(IPP_MODULES_VARIANTS core signal_processing image_processing color_conversion computer_vision vector_math)
set(IPP_MODULES_VARIANTS_STRING "core, signal_processing, image_processing, color_conversion, computer_vision, vector_math")

set(IPP_FOUND FALSE)
set(IPP_INCLUDE_DIRS NOTFOUND)
set(IPP_LIBRARIES "")
set(IPP_LIBRARY_DIRS "")
set(IPP_RUNTIME_LIBRARY_DIRS "")
set(IPP_VERSION_STRING NOTFOUND)
set(IPP_VERSION_MAJOR NOTFOUND)
set(IPP_VERSION_MINOR NOTFOUND)
set(IPP_VERSION_PATCH NOTFOUND)

if(CMAKE_FIND_PACKAGE_NAME)
    if(NOT (${CMAKE_FIND_PACKAGE_NAME} STREQUAL IPP))
        message(FATAL_ERROR "Wrong package name: ${CMAKE_FIND_PACKAGE_NAME}. Must be: IPP.")
    endif(NOT (${CMAKE_FIND_PACKAGE_NAME} STREQUAL IPP))
endif(CMAKE_FIND_PACKAGE_NAME)

macro(IPP_ERROR_MESSAGE MESSAGE)
    if(IPP_FIND_REQUIRED)
        message(FATAL_ERROR ${MESSAGE} ${ARGN})
    elseif(IPP_FIND_QUIETLY)
    else(IPP_FIND_REQUIRED)
        message(WARNING ${MESSAGE} ${ARGN})
    endif(IPP_FIND_REQUIRED)
endmacro(IPP_ERROR_MESSAGE)

macro(IPP_STATUS_MESSAGE MESSAGE)
    if(NOT IPP_FIND_QUIETLY)
        message(STATUS ${MESSAGE} ${ARGN})
    endif(NOT IPP_FIND_QUIETLY)
endmacro(IPP_STATUS_MESSAGE)

unset(IPP_INCLUDE_DIRS CACHE)

find_path(IPP_INCLUDE_DIRS ippversion.h
  HINTS
    ${IPP_PATH}
    ENV IPPPATH
    ENV IPP_PATH
  PATH_SUFFIXES include
)

if(NOT (EXISTS ${IPP_INCLUDE_DIRS}))
    IPP_ERROR_MESSAGE("Not found IPP include directory.")
endif(NOT (EXISTS ${IPP_INCLUDE_DIRS}))

if(EXISTS ${IPP_INCLUDE_DIRS})
    set(VERSION_FILE "${IPP_INCLUDE_DIRS}/ippversion.h")
    set(MAJOR_LEVEL_REGEX "^#define[ \t]+IPP_VERSION_MAJOR[ \t]+([0-9]+)$")
    set(MINOR_LEVEL_REGEX "^#define[ \t]+IPP_VERSION_MINOR[ \t]+([0-9]+)$")
    set(PATCH_LEVEL_REGEX "^#define[ \t]+IPP_VERSION_UPDATE[ \t]+([0-9]+)$")

    if(EXISTS ${VERSION_FILE})
        file(STRINGS ${VERSION_FILE} VERSION_MAJOR_LINE REGEX ${MAJOR_LEVEL_REGEX})
        file(STRINGS ${VERSION_FILE} VERSION_MINOR_LINE REGEX ${MINOR_LEVEL_REGEX})
        file(STRINGS ${VERSION_FILE} VERSION_PATCH_LINE REGEX ${PATCH_LEVEL_REGEX})
        string(REGEX REPLACE "${MAJOR_LEVEL_REGEX}" "\\1" IPP_VERSION_MAJOR "${VERSION_MAJOR_LINE}")
        string(REGEX REPLACE "${MINOR_LEVEL_REGEX}" "\\1" IPP_VERSION_MINOR "${VERSION_MINOR_LINE}")
        string(REGEX REPLACE "${PATCH_LEVEL_REGEX}" "\\1" IPP_VERSION_PATCH "${VERSION_PATCH_LINE}")
        set(IPP_VERSION_STRING ${IPP_VERSION_MAJOR}.${IPP_VERSION_MINOR}.${IPP_VERSION_PATCH})
    endif(EXISTS ${VERSION_FILE})
endif(EXISTS ${IPP_INCLUDE_DIRS})

if(IPP_FIND_VERSION_EXACT)
    if(IPP_FIND_VERSION)
        if(NOT (${IPP_VERSION_STRING} VERSION_EQUAL ${IPP_FIND_VERSION}))
            if(NOT (${IPP_FIND_VERSION_COUNT} EQUAL 3))
                IPP_ERROR_MESSAGE("Requested IPP version has wrong number of version levels. Must be format [MAJOR].[MINOR].[UPDATE]")
            endif(NOT (${IPP_FIND_VERSION_COUNT} EQUAL 3))
            IPP_ERROR_MESSAGE("Requested IPP ${IPP_FIND_VERSION} version. Founded IPP ${IPP_VERSION_STRING} version.")
        endif(NOT (${IPP_VERSION_STRING} VERSION_EQUAL ${IPP_FIND_VERSION}))
    endif(IPP_FIND_VERSION)
endif(IPP_FIND_VERSION_EXACT)

if(${IPP_VERSION_STRING} VERSION_LESS "8.0.0")
    set(IPP_LIB_SUFFIX "_l")
else(${IPP_VERSION_STRING} VERSION_LESS "8.0.0")
    if(MINGW OR WIN32)
        set(IPP_LIB_SUFFIX "mt")
    else(MINGW OR WIN32)
        set(IPP_LIB_SUFFIX "")
    endif(MINGW OR WIN32)
endif(${IPP_VERSION_STRING} VERSION_LESS "8.0.0")

set(IPP_LIB_NAME_CORE "ippcore${IPP_LIB_SUFFIX}")
set(IPP_LIB_NAME_SIGNAL_PROCESSING "ipps${IPP_LIB_SUFFIX}")
set(IPP_LIB_NAME_IMAGE_PROCESSING "ippi${IPP_LIB_SUFFIX}")
set(IPP_LIB_NAME_COLOR_CONVERSION "ippcc${IPP_LIB_SUFFIX}")
set(IPP_LIB_NAME_COMPUTER_VISION "ippcv${IPP_LIB_SUFFIX}")
set(IPP_LIB_NAME_VECTOR_MATH "ippvm${IPP_LIB_SUFFIX}")

if(UNIX)
    if(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "x86_64")
        set(IPP_LIB_PATH_SUFFIXES lib lib/x64 lib/intel64 lib/x86_64-linux-gnu)
        set(IPP_BIN_PATH_SUFFIXES lib lib/x64 lib/intel64 lib/x86_64-linux-gnu)
    elseif(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "x86")
        set(IPP_LIB_PATH_SUFFIXES lib lib/x86 lib/ia32)
        set(IPP_BIN_PATH_SUFFIXES lib lib/x86 lib/ia32)
    endif(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "x86_64")
elseif(WIN32)
    if(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "AMD64")
        set(IPP_LIB_PATH_SUFFIXES lib/x64 lib/intel64)
        set(IPP_BIN_PATH_SUFFIXES bin/x64 bin/intel64)
    elseif(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "x86")
        set(IPP_LIB_PATH_SUFFIXES lib/x86 lib/ia32)
        set(IPP_BIN_PATH_SUFFIXES bin/x86 bin/ia32)
    endif(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "AMD64")
endif(UNIX)

macro(IPP_FIND_AND_ADD_LIBRARY LIB_NAME)
    unset(LIB_FILE_TEMP CACHE)

    find_library(LIB_FILE_TEMP
      NAMES ${IPP_LIB_NAME_${LIB_NAME}}
      HINTS
        ${IPP_PATH}
        ENV IPPPATH
        ENV IPP_PATH
      PATH_SUFFIXES ${IPP_LIB_PATH_SUFFIXES}
    )

    if(NOT LIB_FILE_TEMP)
        IPP_ERROR_MESSAGE("Not found ipp ${LIB_NAME} lib.")
    else(NOT LIB_FILE_TEMP)
        get_filename_component(IPP_DIR_TEMP ${LIB_FILE_TEMP} DIRECTORY)
        list(APPEND IPP_LIBRARY_DIRS ${IPP_DIR_TEMP})
        unset(IPP_DIR_TEMP)
        list(APPEND IPP_LIBRARIES ${LIB_FILE_TEMP})
    endif(NOT LIB_FILE_TEMP)

    if(LIB_FILE_TEMP)
        if(UNIX)
            get_filename_component(IPP_RUNTIME_DIR_TEMP ${LIB_FILE_TEMP} DIRECTORY)
        elseif(MINGW OR WIN32)
            unset(DLL_FILE_TEMP CACHE)
            find_file(DLL_FILE_TEMP
                NAMES ${IPP_LIB_NAME_${LIB_NAME}}.dll
                HINTS
                    ${IPP_PATH}
                    ENV IPPPATH
                    ENV IPP_PATH
                PATH_SUFFIXES ${IPP_BIN_PATH_SUFFIXES}
              )
            get_filename_component(IPP_RUNTIME_DIR_TEMP ${DLL_FILE_TEMP} DIRECTORY)
            unset(DLL_FILE_TEMP CACHE)
        endif(UNIX)
        list(APPEND IPP_RUNTIME_LIBRARY_DIRS ${IPP_RUNTIME_DIR_TEMP})
        unset(IPP_RUNTIME_DIR_TEMP)
    endif(LIB_FILE_TEMP)

    unset(LIB_FILE_TEMP CACHE)
endmacro(IPP_FIND_AND_ADD_LIBRARY)


IPP_STATUS_MESSAGE("Found IPP version: ${IPP_VERSION_STRING}")

macro(IPP_FIND_MODULE MODULE_NAME)
    set(IPP_MODULE_LIB_NAME IPP_${MODULE_NAME})
    string(TOUPPER ${MODULE_NAME} IPP_MODULE_TEMP_STRING)
    IPP_FIND_AND_ADD_LIBRARY(${IPP_MODULE_TEMP_STRING})
    unset(IPP_MODULE_TEMP_STRING)
endmacro(IPP_FIND_MODULE)

foreach(IPP_CUR_COMPONENT ${IPP_FIND_COMPONENTS})
    list(FIND IPP_MODULES_VARIANTS ${IPP_CUR_COMPONENT} index)
    if(${index} EQUAL -1)
        IPP_ERROR_MESSAGE("Not acceptable component name \"${IPP_CUR_COMPONENT}\". Must be one of: ${IPP_MODULES_VARIANTS_STRING}.")
    else(${index} EQUAL -1)
        IPP_FIND_MODULE(${IPP_CUR_COMPONENT})
    endif(${index} EQUAL -1)
    unset(index)
endforeach(IPP_CUR_COMPONENT)

if(IPP_INCLUDE_DIRS AND IPP_LIBRARIES)
    set(IPP_FOUND TRUE)
endif(IPP_INCLUDE_DIRS AND IPP_LIBRARIES)

list(REMOVE_DUPLICATES IPP_LIBRARY_DIRS)
list(REMOVE_DUPLICATES IPP_RUNTIME_LIBRARY_DIRS)

unset(IPP_LIB_SUFFIX)
unset(IPP_LIB_PATH_SUFFIXES)
unset(IPP_BIN_PATH_SUFFIXES)
unset(IPP_MODULES_VARIANTS)
