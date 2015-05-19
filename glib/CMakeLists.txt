project(glib)

set(LIBS
glew
)
if(WIN32)
	list(APPEND LIBS dinput8 dxguid)
endif()

set(SOURCE
#	glew.c
	Gut.cpp
	GutBMP.cpp
#	GutCamera.cpp
	GutDDS.cpp
#	GutDX10.cpp
#	GutDX9.cpp
	GutFileUtility.cpp
	GutFont.cpp
	GutFontUnicodeWin32.cpp
#	GutFont_DX10.cpp
#	GutFont_DX9.cpp
	GutFont_OpenGL.cpp
	GutHeightmap.cpp
#	GutHeightmap_DX10.cpp
#	GutHeightmap_DX9.cpp
	GutHeightmap_OpenGL.cpp
	GutImage.cpp
	GutInput.cpp
	GutMem.cpp
	GutModel.cpp
#	GutModel_DX10.cpp
#	GutModel_DX9.cpp
	GutModel_OpenGL.cpp
#	GutModel_OpenGL_0526.cpp
	GutOpenGL.cpp
	GutParticle.cpp
	GutShadowVolume.cpp
#	GutTexture_DX10.cpp
#	GutTexture_DX9.cpp
	GutTexture_OpenGL.cpp
	GutTGA.cpp
	GutTimer.cpp
	GutUserControl.cpp
	GutWin32.cpp
#	main.cpp
	Matrix4x4_Reference.cpp
#	Matrix4x4_SSE.cpp
	Vector4_Reference.cpp
#	Vector4_SSE.cpp
)

#add_definitions( -DWIN32_MEAN_AND_LEAN )
add_definitions( -DWIN32_MEAN_AND_LEAN -DGLEW_STATIC -DVC_EXTRALEAN -DGLEW_BUILD -D_CRT_SECURE_NO_WARNINGS )


if(UNIX)
	#list(APPEND SOURCE window/lin32/Core.cpp)
	list(APPEND LIBS GL GLU X11)
endif()
if(WIN32)
	#list(APPEND SOURCE window/win32/Core.cpp)
	list(APPEND LIBS opengl32 glu32 gdi32)
endif()

if(WIN32)
	include_directories("D:/DirectXSDK/Include")
	link_directories("D:/DirectXSDK/Lib/x86")
endif()

include_directories( ${ROOT_SOURCE_DIR}/glew/GL )
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
