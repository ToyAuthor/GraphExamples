project(GraphExamples)
cmake_minimum_required(VERSION 2.8)

set(ROOT_SOURCE_DIR ${PROJECT_SOURCE_DIR})
set(ROOT_BINARY_DIR ${PROJECT_BINARY_DIR})

file(COPY ${ROOT_SOURCE_DIR}/shaders DESTINATION ${ROOT_BINARY_DIR}/)
file(COPY ${ROOT_SOURCE_DIR}/textures DESTINATION ${ROOT_BINARY_DIR}/)

if(WIN32)
option(GUT_CROSS_PLATFORM "Try to runing on Linux" OFF)
else()
set(GUT_CROSS_PLATFORM ON)
endif()

add_subdirectory(glew)

if(GUT_CROSS_PLATFORM)
set(GUT_GLIB_INCLUDE_DIR ${ROOT_SOURCE_DIR}/gut)
add_subdirectory(gut)
add_subdirectory(examples)
else()
set(GUT_GLIB_INCLUDE_DIR ${ROOT_SOURCE_DIR}/glib)
add_subdirectory(glib)
add_subdirectory(chap14)
endif()
