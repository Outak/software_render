set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../build-${PROJECT_NAME})
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../build-${PROJECT_NAME})

macro(create_cache_variable NAME)
    unset(${NAME} CACHE)
    set(${NAME} "" CACHE INTERNAL "cached value" FORCE)
endmacro(create_cache_variable)

macro(set_parent_scope VARIABLE)
    get_directory_property(HAS_PARENT PARENT_DIRECTORY)
    if(HAS_PARENT)
        set(${VARIABLE} ${ARGN} PARENT_SCOPE)
    else()
        set(${VARIABLE} ${ARGN})
    endif()
endmacro(set_parent_scope)

macro(append_in_parent_scope VARIABLE) #and list of vars
    list(APPEND ${VARIABLE} ${ARGN})
    set_parent_scope(${VARIABLE} ${${VARIABLE}})
endmacro(append_in_parent_scope)

macro(append_in_cache_scope VARIABLE) #and list of vars
    list(APPEND ${VARIABLE} ${ARGN})
    set(${VARIABLE} ${${VARIABLE}}  CACHE INTERNAL "cached value" FORCE)
endmacro(append_in_cache_scope)

macro(assert_files_exists) #list of files
    foreach(filename ${ARGN})
        if (NOT ((EXISTS ${filename}) OR (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${filename})))
            message(FATAL_ERROR "File ${filename} not exists.")
        endif(NOT ((EXISTS ${filename}) OR (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${filename})))
    endforeach(filename)
endmacro()
