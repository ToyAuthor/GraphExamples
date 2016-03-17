project(glib)

set(LIBS
glew
)


set(SOURCE
	Gut.cpp
	GutWin32.cpp
	Matrix4x4.cpp
	GutUserControl.cpp
	GutOpenGL.cpp
	GutTexture_OpenGL.cpp
	GutFileUtility.cpp
	GutModel.cpp
	GutModel_OpenGL.cpp
	Vector4.cpp
	GutTGA.cpp
	GutBMP.cpp
	GutImage.cpp
	GutTimer.cpp
)

add_definitions( -DWIN32_MEAN_AND_LEAN -DGLEW_STATIC -DVC_EXTRALEAN -DGLEW_BUILD -D_CRT_SECURE_NO_WARNINGS )


if(UNIX)
	#list(APPEND SOURCE window/lin32/Core.cpp)
	list(APPEND LIBS GL GLU X11)
endif()
if(WIN32)
	#list(APPEND SOURCE window/win32/Core.cpp)
	list(APPEND LIBS opengl32 glu32 gdi32)
endif()



include_directories( ${ROOT_SOURCE_DIR}/glew )
link_directories( ${ROOT_BINARY_DIR}/lib )

set(LIBRARY_OUTPUT_PATH ${ROOT_BINARY_DIR}/lib)

add_library( glib STATIC ${SOURCE} )
target_link_libraries( glib ${LIBS} )

if(MSVC)
	set_target_properties( glib PROPERTIES
	LIBRARY_OUTPUT_DIRECTORY            ${CMAKE_CURRENT_BINARY_DIR}
	LIBRARY_OUTPUT_DIRECTORY_DEBUG      ${CMAKE_CURRENT_BINARY_DIR}
	LIBRARY_OUTPUT_DIRECTORY_RELEASE    ${CMAKE_CURRENT_BINARY_DIR}
	RUNTIME_OUTPUT_DIRECTORY            ${CMAKE_CURRENT_BINARY_DIR}
	RUNTIME_OUTPUT_DIRECTORY_DEBUG      ${CMAKE_CURRENT_BINARY_DIR}
	RUNTIME_OUTPUT_DIRECTORY_RELEASE    ${CMAKE_CURRENT_BINARY_DIR}
	ARCHIVE_OUTPUT_DIRECTORY            ${CMAKE_CURRENT_BINARY_DIR}
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG      ${CMAKE_CURRENT_BINARY_DIR}
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE    ${CMAKE_CURRENT_BINARY_DIR}
	)
endif()
