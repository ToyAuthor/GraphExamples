project(Shadow)


#add_definitions( -DWIN32_MEAN_AND_LEAN )
add_definitions( -DWIN32_MEAN_AND_LEAN -DGLEW_STATIC -DVC_EXTRALEAN -DGLEW_BUILD -D_CRT_SECURE_NO_WARNINGS )

include_directories(${ROOT_SOURCE_DIR}/glib)
include_directories(${ROOT_SOURCE_DIR}/glew/GL)
link_directories(${ROOT_BINARY_DIR}/lib)

if(WIN32)
	include_directories("D:/DirectXSDK/Include")
	link_directories("D:/DirectXSDK/Lib/x86")
endif()

#故意弄兩層只是為了滿足原本的資料夾結構，這樣就不用到處修改資源路徑
set(EXECUTABLE_OUTPUT_PATH ${ROOT_BINARY_DIR}/bin/bin)

# Visual Studio才需要在這裡連結dinput8
set(LIBS
glib
glew
opengl32 glu32 gdi32 dinput8 dxguid
)

set(SOURCE
main.cpp
render_data.cpp
render_opengl.cpp
#render_dx9.cpp
)

add_executable(Shadow ${SOURCE})

target_link_libraries(Shadow ${LIBS})

if(MSVC)
	set_target_properties(Shadow PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY            ${ROOT_BINARY_DIR}/bin
		RUNTIME_OUTPUT_DIRECTORY_DEBUG      ${ROOT_BINARY_DIR}/bin
		RUNTIME_OUTPUT_DIRECTORY_RELEASE    ${ROOT_BINARY_DIR}/bin
		ARCHIVE_OUTPUT_DIRECTORY            ${ROOT_BINARY_DIR}/bin
		ARCHIVE_OUTPUT_DIRECTORY_DEBUG      ${ROOT_BINARY_DIR}/bin
		ARCHIVE_OUTPUT_DIRECTORY_RELEASE    ${ROOT_BINARY_DIR}/bin
	)
endif()
