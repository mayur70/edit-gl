cmake_minimum_required(VERSION 3.20)
project(StbImage LANGUAGES C)

add_library(StbImage STATIC
	stb_image.c
)
target_include_directories(StbImage PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

find_library(MATH_LIBRARY m)
if(MATH_LIBRARY)
	target_link_libraries(StbImage PUBLIC ${MATH_LIBRARY})
endif()

