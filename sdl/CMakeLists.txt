start_subdirectory()

add_subdirectory(common)
add_subdirectory(system)
add_subdirectory(control)
add_subdirectory(surface)
add_subdirectory(render)
add_subdirectory(window)
add_subdirectory(texture)
add_subdirectory(color)
add_subdirectory(text)

add_header_file(${TARGET_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/sdl.hpp)

end_subdirectory()
