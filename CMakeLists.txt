cmake_minimum_required(VERSION 2.8)

project(HABR_RENDER)

include(___cmake_auxiliary/CMakeSystemHandler.cmake)
include(___cmake_auxiliary/CMakeAuxiliary.cmake)
include(___cmake_auxiliary/CMakeTargetDefenition.cmake)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/___cmake_modules)

find_package(SDL2 REQUIRED ttf)

set(TARGET_NAME HABR_RENDER)

create_target(${TARGET_NAME} EXEC DEBUG "")

add_subdirectory(sdl)

add_header_file(${TARGET_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/geometry.h)

add_source_file(${TARGET_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

add_lib_file(${TARGET_NAME} mingw32)
add_lib_file(${TARGET_NAME} ${SDL2_LIBRARIES})
add_include_path(${TARGET_NAME} ${SDL2_INCLUDE_DIR})

add_compiler_options(${TARGET_NAME} -std=c++11)

assemble_all_targets()

