cmake_minimum_required(VERSION 3.20)

project(Glad LANGUAGES C)

add_library(Glad STATIC
	"src/glad.c"
)

target_include_directories(Glad PUBLIC "include")
