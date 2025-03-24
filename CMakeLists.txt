cmake_minimum_required(VERSION 3.20)

project(edit-gl LANGUAGES C)

add_executable(edit-gl
	main.c
)

set_property(TARGET edit-gl PROPERTY CMAKE_C_STANDARD 17)
set_property(TARGET edit-gl PROPERTY CMAKE_C_STANDARD_REQUIRED TRUE)

target_compile_options(edit-gl PRIVATE -Wall -Wextra -pedantic)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory("vendor")

target_link_libraries(edit-gl
	PRIVATE
	glfw
	OpenGL::GL
	Glad
)

add_custom_command(TARGET edit-gl POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/res
    $<TARGET_FILE_DIR:edit-gl>/res
)
