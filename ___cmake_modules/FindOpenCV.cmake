#.rst:
# FindOpenCV
# -------
# - Find OpenCV library and modules
#
# Use this module by invoking find_package with the form::
#
#   find_package(OpenCV
#     [version] [EXACT]      # Minimum or EXACT version e.g. 2.7.1
#     [REQUIRED]             # Fail with error if any of SDL modules not found
#     [COMPONENTS <libs>...] # OpenCV modules by their name
#     )                      #
#
#  possible components -- imgproc, highgui, legacy, gpu, ml(machine learning), video
#
# This module defines
#  OPENCV_FOUND -- set false if not found OpenCV
#  OPENCV_[MODULE NAME]_FOUND -- set false if not found module
#  OPENCV_INCLUDE_DIR -- include directory where is can be found directory opencv2/ not *.h files
#  OPENCV_LIBRARIES -- list of the libraries to link
#  OPENCV_RUNTIME_LIBRARY_DIRS -- directory of shared libs *.so (or *.dll files for windows) for program launch
#  OPENCV_VERSION_STRING -- string containing the version of module
#  OPENCV_VERSION_EPOCH -- epoch version of module
#  OPENCV_VERSION_MAJOR -- major version of module
#  OPENCV_VERSION_MINOR -- minor version of module
#  OPENCV_VERSION_PATCH -- patch version of module
#
#  $OPENCV_ROOT $OPENCV_PATH is an environment variable where OpenCV was installed
#

set(OPENCV_FOUND FALSE)
set(OPENCV_IMGPROC_FOUND FALSE)
set(OPENCV_HIGHGUI_FOUND FALSE)
set(OPENCV_LEGACY_FOUND FALSE)
set(OPENCV_GPU_FOUND FALSE)
set(OPENCV_ML_FOUND FALSE)
set(OPENCV_VIDEO_FOUND FALSE)

set(OPENCV_INCLUDE_DIR NOTFOUND)
set(OPENCV_LIBRARIES "")
set(OPENCV_RUNTIME_LIBRARY_DIRS "")

set(OPENCV_VERSION_STRING NOTFOUND)
set(OPENCV_VERSION_MAJOR NOTFOUND)
set(OPENCV_VERSION_MINOR NOTFOUND)
set(OPENCV_VERSION_PATCH NOTFOUND)

set(OPENCV_MODULES_VARIANTS imgproc highgui legacy gpu ml video)

if(CMAKE_FIND_PACKAGE_NAME)
    if(NOT (${CMAKE_FIND_PACKAGE_NAME} STREQUAL OpenCV))
        message(FATAL_ERROR "Wrong package name: ${CMAKE_FIND_PACKAGE_NAME}. Must be: OpenCV.")
    endif(NOT (${CMAKE_FIND_PACKAGE_NAME} STREQUAL OpenCV))
endif(CMAKE_FIND_PACKAGE_NAME)

macro(OPENCV_ERROR_MESSAGE MESSAGE)
    if(OpenCV_FIND_REQUIRED)
        message(FATAL_ERROR ${MESSAGE} ${ARGN})
    elseif(OpenCV_FIND_QUIETLY)
    else(OpenCV_FIND_REQUIRED)
        message(WARNING ${MESSAGE} ${ARGN})
    endif(OpenCV_FIND_REQUIRED)
endmacro(OPENCV_ERROR_MESSAGE)

macro(OPENCV_STATUS_MESSAGE MESSAGE)
    if(NOT OpenCV_FIND_QUIETLY)
        message(STATUS ${MESSAGE} ${ARGN})
    endif(NOT OpenCV_FIND_QUIETLY)
endmacro(OPENCV_STATUS_MESSAGE)

function(FINE_LIST_PRINT DST DELIMITER)
    list(GET ARGV 2 temp)
    math(EXPR N "${ARGC}-1")
    foreach(IDX RANGE 3 ${N})
        list(GET ARGV ${IDX} STR)
        set(temp "${temp}${DELIMITER}${STR}")
    endforeach()
    set(${DST} "${temp}" PARENT_SCOPE)
endfunction(FINE_LIST_PRINT DST DELIMITER)

if(UNIX)
    if(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "x86_64")
        set(OPENCV_LIB_PATH_SUFFIXES lib lib/x64 lib/intel64 lib/x86_64-linux-gnu)
        set(OPENCV_BIN_PATH_SUFFIXES lib lib/x64 lib/intel64 lib/x86_64-linux-gnu)
    elseif(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "x86")
        set(OPENCV_LIB_PATH_SUFFIXES lib lib/x86 lib/ia32)
        set(OPENCV_BIN_PATH_SUFFIXES lib lib/x86 lib/ia32)
    endif(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "x86_64")
elseif(WIN32)
    if(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "AMD64")
        set(OPENCV_LIB_PATH_SUFFIXES lib/x64 lib/intel64)
        set(OPENCV_BIN_PATH_SUFFIXES bin/x64 bin/intel64)
    elseif(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "x86")
        set(OPENCV_LIB_PATH_SUFFIXES lib/x86 lib/ia32)
        set(OPENCV_BIN_PATH_SUFFIXES bin/x86 bin/ia32)
    endif(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "AMD64")
endif(UNIX)

macro(OPENCV_FIND_AND_ADD_LIBRARY LIB_NAME LIB_FOUNDED)
    unset(${LIB_NAME}_LIB CACHE)

    find_library(${LIB_NAME}_LIB
      NAMES ${LIB_NAME}
      HINTS
        ENV OPENCV_ROOT
        ENV OPENCV_PATH
      PATH_SUFFIXES ${OPENCV_LIB_PATH_SUFFIXES}
    )

    if(NOT ${LIB_NAME}_LIB)
        OPENCV_ERROR_MESSAGE("Not found ${LIB_NAME} lib.")
        set(${LIB_FOUNDED} FALSE)
    else(NOT ${LIB_NAME}_LIB)
        list(APPEND OPENCV_LIBRARIES ${${LIB_NAME}_LIB})
        set(${LIB_FOUNDED} TRUE)
    endif(NOT ${LIB_NAME}_LIB)

    if(${LIB_NAME}_LIB)
        if(UNIX)
            get_filename_component(OPENCV_RUNTIME_DIR_TEMP ${${LIB_NAME}_LIB} DIRECTORY)
        elseif(MINGW OR WIN32)
            unset(${LIB_NAME}_DLL CACHE)
            find_file(${LIB_NAME}_DLL
                NAMES ${LIB_NAME}.dll
                HINTS
                    ENV OPENCV_ROOT
                    ENV OPENCV_PATH
                PATH_SUFFIXES ${OPENCV_BIN_PATH_SUFFIXES}
              )
            get_filename_component(OPENCV_RUNTIME_DIR_TEMP ${LIB_NAME}_DLL DIRECTORY)
            unset(${LIB_NAME}_DLL CACHE)
        endif(UNIX)
        list(APPEND OPENCV_RUNTIME_LIBRARY_DIRS ${OPENCV_RUNTIME_DIR_TEMP})
        unset(OPENCV_RUNTIME_DIR_TEMP)
    endif(${LIB_NAME}_LIB)

    unset(${LIB_NAME}_LIB CACHE)
endmacro(OPENCV_FIND_AND_ADD_LIBRARY)

unset(OPENCV_INCLUDE_DIR_TEMP CACHE)

find_path(OPENCV_INCLUDE_DIR_TEMP opencv.hpp
  HINTS
      ENV OPENCV_ROOT
      ENV OPENCV_PATH
  PATH_SUFFIXES include/opencv2
)

if(NOT (EXISTS ${OPENCV_INCLUDE_DIR_TEMP}))
    OPENCV_ERROR_MESSAGE("Not found OpenCV include directory")
else(NOT (EXISTS ${OPENCV_INCLUDE_DIR_TEMP}))
    get_filename_component(OPENCV_INCLUDE_DIR_TEMP ${OPENCV_INCLUDE_DIR_TEMP} DIRECTORY)
    set(OPENCV_INCLUDE_DIR ${OPENCV_INCLUDE_DIR_TEMP})
endif(NOT (EXISTS ${OPENCV_INCLUDE_DIR_TEMP}))

unset(OPENCV_INCLUDE_DIR_TEMP CACHE)

if(EXISTS ${OPENCV_INCLUDE_DIR})
    set(VERSION_FILE "${OPENCV_INCLUDE_DIR}/opencv2/core/version.hpp")
    set(EPOCH_LEVEL_REGEX "^#define[ \t]+CV_VERSION_EPOCH[ \t]+([0-9]+)$")
    set(MAJOR_LEVEL_REGEX "^#define[ \t]+CV_VERSION_MAJOR[ \t]+([0-9]+)$")
    set(MINOR_LEVEL_REGEX "^#define[ \t]+CV_VERSION_MINOR[ \t]+([0-9]+)$")
    set(PATCH_LEVEL_REGEX "^#define[ \t]+CV_VERSION_REVISION[ \t]+([0-9]+)$")

    if(EXISTS ${VERSION_FILE})
        file(STRINGS ${VERSION_FILE} VERSION_EPOCH_LINE REGEX ${EPOCH_LEVEL_REGEX})
        file(STRINGS ${VERSION_FILE} VERSION_MAJOR_LINE REGEX ${MAJOR_LEVEL_REGEX})
        file(STRINGS ${VERSION_FILE} VERSION_MINOR_LINE REGEX ${MINOR_LEVEL_REGEX})
        file(STRINGS ${VERSION_FILE} VERSION_PATCH_LINE REGEX ${PATCH_LEVEL_REGEX})
        string(REGEX REPLACE "${EPOCH_LEVEL_REGEX}" "\\1" OPENCV_VERSION_EPOCH "${VERSION_EPOCH_LINE}")
        string(REGEX REPLACE "${MAJOR_LEVEL_REGEX}" "\\1" OPENCV_VERSION_MAJOR "${VERSION_MAJOR_LINE}")
        string(REGEX REPLACE "${MINOR_LEVEL_REGEX}" "\\1" OPENCV_VERSION_MINOR "${VERSION_MINOR_LINE}")
        string(REGEX REPLACE "${PATCH_LEVEL_REGEX}" "\\1" OPENCV_VERSION_PATCH "${VERSION_PATCH_LINE}")
        set(OPENCV_VERSION_STRING ${OPENCV_VERSION_EPOCH}.${OPENCV_VERSION_MAJOR}.${OPENCV_VERSION_MINOR}.${OPENCV_VERSION_PATCH})
    endif(EXISTS ${VERSION_FILE})
endif(EXISTS ${OPENCV_INCLUDE_DIR})

OPENCV_FIND_AND_ADD_LIBRARY(opencv_core OPENCV_FOUND)

if(OpenCV_FIND_VERSION_EXACT)
    if(OpenCV_FIND_VERSION)
        if(NOT (${OPENCV_VERSION_STRING} VERSION_EQUAL ${OpenCV_FIND_VERSION}))
            if(NOT (${OpenCV_FIND_VERSION_COUNT} EQUAL 3))
                OPENCV_ERROR_MESSAGE("Requested OpenCV version has wrong number of version levels. Must be format [MAJOR].[MINOR].[UPDATE]")
            endif(NOT (${OpenCV_FIND_VERSION_COUNT} EQUAL 3))
            OPENCV_ERROR_MESSAGE("Requested OpenCV ${OpenCV_FIND_VERSION} version. Founded OpenCV ${OPENCV_VERSION_STRING} version.")
        endif(NOT (${OpenCV_VERSION_STRING} VERSION_EQUAL ${OpenCV_FIND_VERSION}))
    endif(OpenCV_FIND_VERSION)
endif(OpenCV_FIND_VERSION_EXACT)

OPENCV_STATUS_MESSAGE("Found OpenCV version: ${OPENCV_VERSION_STRING}")

macro(OPENCV_FIND_MODULE MODULE_NAME)
    set(OPENCV_MODULE_LIB_NAME opencv_${MODULE_NAME})
    string(TOUPPER ${MODULE_NAME} OPENCV_MODULE_TEMP_STRING)
    OPENCV_FIND_AND_ADD_LIBRARY(${OPENCV_MODULE_LIB_NAME} OPENCV_${MODULE_NAME}_FOUND)
endmacro(OPENCV_FIND_MODULE)

foreach(OPENCV_CUR_COMPONENT ${OPENCV_FIND_COMPONENTS})
    list(FIND OPENCV_MODULES_VARIANTS ${OPENCV_CUR_COMPONENT} index)
    if(${index} EQUAL -1)
        FINE_LIST_PRINT(FINE_TMP_LIST ", " ${OPENCV_MODULES_VARIANTS})
        OPENCV_ERROR_MESSAGE("Not acceptable component name \"${OPENCV_CUR_COMPONENT}\". Must be one of: ${FINE_TMP_LIST}.")
        unset(FINE_TMP_LIST)
    else(${index} EQUAL -1)
        OPENCV_FIND_MODULE(${OPENCV_CUR_COMPONENT})
    endif(${index} EQUAL -1)
    unset(index)
endforeach(OPENCV_CUR_COMPONENT)

list(REMOVE_DUPLICATES OPENCV_RUNTIME_LIBRARY_DIRS)

unset(OPENCV_MODULES_VARIANTS)
