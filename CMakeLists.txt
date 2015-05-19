project(GraphExamples)
cmake_minimum_required(VERSION 2.8)

set(ROOT_SOURCE_DIR ${PROJECT_SOURCE_DIR})
set(ROOT_BINARY_DIR ${PROJECT_BINARY_DIR})

file(COPY ${ROOT_SOURCE_DIR}/shaders DESTINATION ${ROOT_BINARY_DIR}/)
file(COPY ${ROOT_SOURCE_DIR}/textures DESTINATION ${ROOT_BINARY_DIR}/)

add_subdirectory(glew)
add_subdirectory(glib)
add_subdirectory(chap14)
