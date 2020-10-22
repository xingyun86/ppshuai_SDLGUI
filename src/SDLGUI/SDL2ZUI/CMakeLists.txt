# CMakeList.txt : CMake project for sdlgui, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project(sdl2zui C)

include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/zui
	${CMAKE_SOURCE_DIR}/base
	${CMAKE_SOURCE_DIR}/base/malloc
	${CMAKE_SOURCE_DIR}/tinyxml2
	${CMAKE_BINARY_DIR}
	${CMAKE_SOURCE_DIR}/SDL2
	${CMAKE_SOURCE_DIR}/SDL2/include
	${CMAKE_BINARY_DIR}/SDL2
	${CMAKE_SOURCE_DIR}/SDL2_gfx
	${CMAKE_BINARY_DIR}/SDL2_gfx
	${CMAKE_SOURCE_DIR}/SDL2_gif
	${CMAKE_BINARY_DIR}/SDL2_gif
	${CMAKE_SOURCE_DIR}/SDL2_ttf
	${CMAKE_BINARY_DIR}/SDL2_ttf
	${CMAKE_SOURCE_DIR}/SDL2_net
	${CMAKE_BINARY_DIR}/SDL2_net
	${CMAKE_SOURCE_DIR}/SDL2_image
	${CMAKE_BINARY_DIR}/SDL2_image
	${CMAKE_SOURCE_DIR}/SDL2_mixer
	${CMAKE_BINARY_DIR}/SDL2_mixer
	${CMAKE_SOURCE_DIR}/mpg123/src/libmpg123
	${CMAKE_BINARY_DIR}/mpg123/src/libmpg123
	${CMAKE_SOURCE_DIR}/libmodplug/src
	${CMAKE_BINARY_DIR}/libmodplug/src
	${CMAKE_SOURCE_DIR}/libvorbisidec
	${CMAKE_BINARY_DIR}/libvorbisidec
	${CMAKE_SOURCE_DIR}/vorbis/include
	${CMAKE_BINARY_DIR}/vorbis
)
link_directories(
	${CMAKE_BINARY_DIR}/SDL2
	${CMAKE_BINARY_DIR}/SDL2_gfx
	${CMAKE_BINARY_DIR}/SDL2_gif
	${CMAKE_BINARY_DIR}/SDL2_ttf
	${CMAKE_BINARY_DIR}/SDL2_net
	${CMAKE_BINARY_DIR}/SDL2_image
	${CMAKE_BINARY_DIR}/SDL2_mixer
	${CMAKE_BINARY_DIR}/mpg123/src/libmpg123
	${CMAKE_BINARY_DIR}/libmodplug/src
	${CMAKE_BINARY_DIR}/libvorbisidec
	${CMAKE_BINARY_DIR}/vorbis/lib
)
# Add source to this project's executable.
add_executable (sdl2zui 
	"ZUI/main.cpp" 
	"ZUI/ZApplication.cpp" 
	"ZUI/ZBuilder.cpp" 
	"ZUI/ZButton.cpp" 
	"ZUI/ZCommon.cpp" 
	"ZUI/ZContainer.cpp" 
	"ZUI/ZControl.cpp" 
	"ZUI/ZControlFactory.cpp" 
	"ZUI/ZFont.cpp" 
	"ZUI/ZHorizontalLayout.cpp" 
	"ZUI/ZLabel.cpp" 
	"ZUI/ZLayout.cpp" 
	"ZUI/ZOption.cpp" 
	"ZUI/ZPaintManager.cpp" 
	"ZUI/ZProgress.cpp" 
	"ZUI/ZRenerder.cpp" 
	"ZUI/ZScrollBar.cpp" 
	"ZUI/ZSlider.cpp" 
	"ZUI/ZString.cpp" 
	"ZUI/ZTabLayout.cpp" 
	"ZUI/ZTexture.cpp" 
	"ZUI/ZUtils.cpp" 
	"ZUI/ZVerticalLayout.cpp" 
	"ZUI/ZWindow.cpp" 
	"tinyxml2/tinyxml2.cpp"
	"base/malloc/nedmalloc.c"
	"base/common.cc"
	"base/event.cc"
	"base/messagequeue.cc"
	"base/RefBase.cpp"
	"base/thread.cc"
	"base/time.cc"
)

list(APPEND EXT_LIB)

if (MSVC)
	list(APPEND project_name_cflags /W4)
	list(APPEND project_name_defines WIN32_LEAN_AND_MEAN _WIN32_WINNT=0x0501)
	MESSAGE(STATUS "CMAKE_BUILD_TYPE=" ${CMAKE_BUILD_TYPE})

	set(CompilerFlags
		CMAKE_CXX_FLAGS
		CMAKE_CXX_FLAGS_DEBUG
		CMAKE_CXX_FLAGS_RELEASE
		CMAKE_C_FLAGS
		CMAKE_C_FLAGS_DEBUG
		CMAKE_C_FLAGS_RELEASE
		)
	foreach(CompilerFlag ${CompilerFlags})
		string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
	endforeach()
	
	target_compile_options(sdl2zui
							PRIVATE ${project_name_cflags}
	)
	target_compile_definitions(sdl2zui
							PRIVATE ${project_name_defines}
							)
	list(APPEND EXT_LIB iphlpapi ws2_32)
else()
    set(CMAKE_C_FLAGS "$ENV{CFLAGS} ${CMAKE_C_FLAGS} -fPIC")
    set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} ${CMAKE_CXX_FLAGS} -fPIC")
	list(APPEND EXT_LIB pthread dl)
endif(MSVC)

target_link_libraries(sdl2zui 
	SDL2_static 
	SDL2_gfx
	SDL2_gif
	SDL2_ttf 
	SDL2_net 
	SDL2_image  
	SDL2_mixer
	mpg123
	modplug
	vorbisidec
	${EXT_LIB}
)

# TODO: Add tests and install targets if needed.
