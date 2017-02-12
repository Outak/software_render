create_cache_variable(__AUX_ALL_TARGETS_NAMES "list of all targets")


macro(create_target TARGET_NAME BIN_TYPE BUILD_TYPE DEPENDENCIES_TARGET_LIST)
    if ((${BIN_TYPE} STREQUAL EXEC))
    elseif ((${BIN_TYPE} STREQUAL LIB_STATIC))
    elseif (NOT (${BIN_TYPE} STREQUAL LIB_SHARED))
        message(FATAL_ERROR "${TARGET_NAME} has wrong BIN_TYPE parameter")
    endif((${BIN_TYPE} STREQUAL EXEC))

    if ((${BUILD_TYPE} STREQUAL DEBUG))
    elseif (NOT (${BUILD_TYPE} STREQUAL RELEASE))
        message(FATAL_ERROR "${TARGET_NAME} has wrong BUILD_TYPE = ${BUILD_TYPE} parameter")
    endif((${BUILD_TYPE} STREQUAL DEBUG))

    create_cache_variable(_private_${TARGET_NAME}_BIN_TYPE)
    create_cache_variable(_private_${TARGET_NAME}_BUILD_TYPE)
    create_cache_variable(_private_${TARGET_NAME}_DEPENDENCIES)
    create_cache_variable(_private_${TARGET_NAME}_HEADERS)
    create_cache_variable(_private_${TARGET_NAME}_SOURCES)
    create_cache_variable(_private_${TARGET_NAME}_LIBRARIES)
    create_cache_variable(_private_${TARGET_NAME}_LIBRARIES_DIRS)
    create_cache_variable(_private_${TARGET_NAME}_INCLUDES)
    create_cache_variable(_private_${TARGET_NAME}_COMPILE_OPTIONS)
    create_cache_variable(_private_${TARGET_NAME}_DEFINITIONS)

    set(_private_${TARGET_NAME}_BIN_TYPE ${BIN_TYPE} CACHE INTERNAL "cached value" FORCE)
    set(_private_${TARGET_NAME}_BUILD_TYPE ${BUILD_TYPE} CACHE INTERNAL "cached value" FORCE)

    append_in_cache_scope(__AUX_ALL_TARGETS_NAMES ${TARGET_NAME})
endmacro(create_target)

macro(assemble_executable_target TARGET_NAME)
    if (_private_${TARGET_NAME}_BUILD_TYPE STREQUAL DEBUG)
        list(APPEND _private_${TARGET_NAME}_COMPILE_OPTIONS -g3)
    elseif(_private_${TARGET_NAME}_BUILD_TYPE STREQUAL RELEASE)
        list(APPEND _private_${TARGET_NAME}_COMPILE_OPTIONS -O2)
    endif(_private_${TARGET_NAME}_BUILD_TYPE STREQUAL DEBUG)

    add_executable(${TARGET_NAME} ${_private_${TARGET_NAME}_HEADERS} ${_private_${TARGET_NAME}_SOURCES})
    target_compile_options(${TARGET_NAME} PUBLIC -Wall ${_private_${TARGET_NAME}_COMPILE_OPTIONS})
    target_compile_definitions(${TARGET_NAME} PUBLIC ${_private_${TARGET_NAME}_DEFINITIONS})
    target_include_directories(${TARGET_NAME} PUBLIC ${PROJECT_SOURCE_DIR} ${_private_${TARGET_NAME}_INCLUDES})
    target_link_libraries(${TARGET_NAME} ${_private_${TARGET_NAME}_LIBRARIES})
    target_link_libraries(${TARGET_NAME} -pthread)
endmacro(assemble_executable_target)

macro(assemble_library_target TARGET_NAME LIBTYPE)
    if (_private_${TARGET_NAME}_BUILD_TYPE STREQUAL DEBUG)
        list(APPEND _private_${TARGET_NAME}_COMPILE_OPTIONS -g3)
    elseif(_private_${TARGET_NAME}_BUILD_TYPE STREQUAL RELEASE)
        list(APPEND _private_${TARGET_NAME}_COMPILE_OPTIONS -O2)
    endif(_private_${TARGET_NAME}_BUILD_TYPE STREQUAL DEBUG)

    add_library(${TARGET_NAME} ${LIBTYPE} ${_private_${TARGET_NAME}_HEADERS} ${_private_${TARGET_NAME}_SOURCES})
    target_compile_options(${TARGET_NAME} PUBLIC -Wall ${_private_${TARGET_NAME}_COMPILE_OPTIONS})
    target_compile_definitions(${TARGET_NAME} PUBLIC ${_private_${TARGET_NAME}_DEFINITIONS})
    target_include_directories(${TARGET_NAME} PUBLIC ${PROJECT_SOURCE_DIR} ${_private_${TARGET_NAME}_INCLUDES})
    target_link_libraries(${TARGET_NAME} ${_private_${TARGET_NAME}_LIBRARIES})
    if(UNIX OR MINGW)
        target_link_libraries(${TARGET_NAME} -pthread)
    endif(UNIX OR MINGW)
endmacro(assemble_library_target)

macro(assemble_all_targets)
    foreach(CURRENT_TARGET ${__AUX_ALL_TARGETS_NAMES})
        if (${_private_${CURRENT_TARGET}_BIN_TYPE} STREQUAL EXEC)
            assemble_executable_target(${CURRENT_TARGET})
        elseif(${_private_${CURRENT_TARGET}_BIN_TYPE} STREQUAL LIB_STATIC)
            assemble_library_target(${CURRENT_TARGET} STATIC)
        elseif(${_private_${CURRENT_TARGET}_BIN_TYPE} STREQUAL LIB_SHARED)
            assemble_library_target(${CURRENT_TARGET} SHARED)
        endif(${_private_${CURRENT_TARGET}_BIN_TYPE} STREQUAL EXEC)
    endforeach(CURRENT_TARGET)
endmacro(assemble_all_targets)

macro(generate_gnu_make_files TARGET_NAME BINARY_NAME)
    SET(makefile_name "${PROJECT_SOURCE_DIR}/makefile")
    file(WRITE ${makefile_name} "program_name = ${BINARY_NAME}\n\n")
    file(APPEND ${makefile_name} "echo = /bin/echo\n\n")

    foreach(CURRENT_DEF ${_private_${TARGET_NAME}_DEFINITIONS})
        file(APPEND ${makefile_name} "DEFINES += -D${CURRENT_DEF}\n")
    endforeach(CURRENT_DEF)

    file(APPEND ${makefile_name} "\n")
    file(APPEND ${makefile_name} "CXXFLAGS += $(DEFINES)\n")

    file(APPEND ${makefile_name} "CXXFLAGS += ")
    foreach(CURRENT_CMP_OPT ${_private_${TARGET_NAME}_COMPILE_OPTIONS})
        file(APPEND ${makefile_name} "${CURRENT_CMP_OPT} ")
    endforeach(CURRENT_CMP_OPT)

    file(APPEND ${makefile_name} "\n")
    if (_private_${TARGET_NAME}_INCLUDES)
        foreach(CURRENT_INC_DIR ${_private_${TARGET_NAME}_INCLUDES})
            file(APPEND ${makefile_name} "CXXFLAGS += -I${CURRENT_INC_DIR}\n")
        endforeach(CURRENT_INC_DIR)
    endif(_private_${TARGET_NAME}_INCLUDES)
    file(APPEND ${makefile_name} "\n")

    foreach(CURRENT_LIB ${_private_${TARGET_NAME}_LIBRARIES})
        get_filename_component(TMP_LIB_LINE ${CURRENT_LIB} NAME)
        string(REGEX REPLACE "lib" "" TMP_LIB_LINE "${TMP_LIB_LINE}")
        string(REGEX REPLACE ".so" "" TMP_LIB_LINE "${TMP_LIB_LINE}")
        file(APPEND ${makefile_name} "LIBS += -l${TMP_LIB_LINE}\n")
    endforeach(CURRENT_LIB)
    unset(${TMP_LIB_LINE})

    file(APPEND ${makefile_name} "\n")
    foreach(CURRENT_LIB_DIR ${_private_${TARGET_NAME}_LIBRARIES_DIRS})
        file(APPEND ${makefile_name} "LDFLAGS += -L${CURRENT_LIB_DIR}\n")
    endforeach(CURRENT_LIB_DIR)
    file(APPEND ${makefile_name} "LDFLAGS += $(LIBS)\n")
    file(APPEND ${makefile_name} "\n")

    foreach(CURRENT_SRC ${_private_${TARGET_NAME}_SOURCES})
        string(REGEX REPLACE "${PROJECT_SOURCE_DIR}/" "" TMP_SRC_LINE "${CURRENT_SRC}")
        string(REGEX REPLACE "\\.cpp|\\.c" ".o" TMP_SRC_LINE "${TMP_SRC_LINE}")
        file(APPEND ${makefile_name} "OBJ += ${TMP_SRC_LINE}\n")
    endforeach(CURRENT_SRC)
    unset(${TMP_SRC_LINE})

    file(APPEND ${makefile_name} "\n")
    file(APPEND ${makefile_name} "all : $(program_name)\n")
    file(APPEND ${makefile_name} "$(program_name) : $(OBJ)\n")
    file(APPEND ${makefile_name} "\t$(CXX) -o $@ $(OBJ) $(LDFLAGS)\n")
    file(APPEND ${makefile_name} "\t@$(echo) -n -e \"\\n-----------| BUILD READY!!! |-----------\\n\"\n")
    file(APPEND ${makefile_name} "\n")
    file(APPEND ${makefile_name} "date: \n")
    file(APPEND ${makefile_name} "\t$ date 010100002042.00\n")
    file(APPEND ${makefile_name} "\n")
    file(APPEND ${makefile_name} "clean:\n")
    file(APPEND ${makefile_name} "\t$ rm -f $(OBJ) $(program_name)\n")
    file(APPEND ${makefile_name} "\n")
endmacro(generate_gnu_make_files)

macro(add_header_file TARGET_NAME) #and list of files
    assert_files_exists(${ARGN})
    append_in_cache_scope(_private_${TARGET_NAME}_HEADERS ${ARGN})
endmacro(add_header_file)

macro(add_source_file TARGET_NAME) #and list of files
    assert_files_exists(${ARGN})
    append_in_cache_scope(_private_${TARGET_NAME}_SOURCES ${ARGN})
endmacro(add_source_file)

macro(add_lib_file TARGET_NAME) #and list of files
    append_in_cache_scope(_private_${TARGET_NAME}_LIBRARIES ${ARGN})
endmacro(add_lib_file)

macro(add_lib_directory TARGET_NAME) #and list of files
    append_in_cache_scope(_private_${TARGET_NAME}_LIBRARIES_DIRS ${ARGN})
endmacro(add_lib_directory)

macro(add_include_path TARGET_NAME) #and list of paths
    append_in_cache_scope(_private_${TARGET_NAME}_INCLUDES ${ARGN})
endmacro(add_include_path)

macro(add_compiler_options TARGET_NAME) #and list of options
    append_in_cache_scope(_private_${TARGET_NAME}_COMPILE_OPTIONS ${ARGN})
endmacro(add_compiler_options)

macro(add_compile_definitions TARGET_NAME) #and list of definitions
    append_in_cache_scope(_private_${TARGET_NAME}_DEFINITIONS ${ARGN})
endmacro(add_compile_definitions)

macro(start_subdirectory)
#    message(STATUS "Enter in ${CMAKE_CURRENT_SOURCE_DIR}.")
endmacro(start_subdirectory)

macro(end_subdirectory)
#    message(STATUS "Exit from ${CMAKE_CURRENT_SOURCE_DIR}.")
endmacro(end_subdirectory)
