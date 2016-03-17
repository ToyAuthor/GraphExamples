
set(PROJECT_NAME "Shadow")

project(${PROJECT_NAME})

add_definitions( -DWIN32_MEAN_AND_LEAN -DGLEW_STATIC -DVC_EXTRALEAN -DGLEW_BUILD -D_CRT_SECURE_NO_WARNINGS )

include_directories(${GUT_GLIB_INCLUDE_DIR})
include_directories(${ROOT_SOURCE_DIR}/glew)
link_directories(${ROOT_BINARY_DIR}/lib)


#故意弄兩層只是為了滿足原本的資料夾結構，這樣就不用到處修改資源路徑
set(EXECUTABLE_OUTPUT_PATH ${ROOT_BINARY_DIR}/bin/bin)


set(LIBS
glib
glew
)

if(UNIX)
list(APPEND LIBS GL GLU X11)
endif()
if(WIN32)
list(APPEND LIBS opengl32 glu32 gdi32)
endif()

set(SOURCE
main.cpp
render_data.cpp
render_opengl.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE})

target_link_libraries(${PROJECT_NAME} ${LIBS})

if(MSVC)
	set_target_properties(${PROJECT_NAME} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY            ${ROOT_BINARY_DIR}/bin
		RUNTIME_OUTPUT_DIRECTORY_DEBUG      ${ROOT_BINARY_DIR}/bin
		RUNTIME_OUTPUT_DIRECTORY_RELEASE    ${ROOT_BINARY_DIR}/bin
		ARCHIVE_OUTPUT_DIRECTORY            ${ROOT_BINARY_DIR}/bin
		ARCHIVE_OUTPUT_DIRECTORY_DEBUG      ${ROOT_BINARY_DIR}/bin
		ARCHIVE_OUTPUT_DIRECTORY_RELEASE    ${ROOT_BINARY_DIR}/bin
	)
endif()
