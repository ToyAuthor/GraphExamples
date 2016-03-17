
set(PROJECT_NAME "XWindow")

project(${PROJECT_NAME})



#故意弄兩層只是為了滿足原本的資料夾結構，這樣就不用到處修改資源路徑
set(EXECUTABLE_OUTPUT_PATH ${ROOT_BINARY_DIR}/bin/bin)

add_definitions(-D_X11_)

set(LIBS
	X11
)


set(SOURCE
main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE})

target_link_libraries(${PROJECT_NAME} ${LIBS})

