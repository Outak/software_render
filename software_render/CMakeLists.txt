start_subdirectory()

add_header_file(${TARGET_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/software_render.hpp)

add_header_file(${TARGET_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/common.hpp)
add_header_file(${TARGET_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/line.hpp)
add_header_file(${TARGET_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/triangle.hpp)
add_header_file(${TARGET_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/mesh.hpp)
add_header_file(${TARGET_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/surf.hpp)
add_header_file(${TARGET_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/zbuffer.hpp)

end_subdirectory()
