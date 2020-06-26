# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("SDLGUI")

# Include sub-projects.
add_subdirectory ("zlib-1.2.11")
add_subdirectory ("giflib-5.2.1")
add_subdirectory ("libiconv-1.16")
add_subdirectory ("ogg")
add_subdirectory ("flac")
add_subdirectory ("opus")
add_subdirectory ("vorbis")
add_subdirectory ("libmodplug")
add_subdirectory ("mpg123")
add_subdirectory ("libvorbisidec")
add_subdirectory ("freetype2")
add_subdirectory ("libwebp")
add_subdirectory ("libtiff-4.1.0")
add_subdirectory ("libjpeg-turbo")
add_subdirectory ("libpng")
add_subdirectory ("SDL2")
add_subdirectory ("timidity")
add_subdirectory ("SDL2_gif")
add_subdirectory ("SDL2_ttf")
add_subdirectory ("SDL2_net")
add_subdirectory ("SDL2_image")
add_subdirectory ("SDL2_mixer")
add_subdirectory ("SDL2GUI")
add_subdirectory ("SDL2GamePingPong")
add_subdirectory ("SDL2GameLinkLook")
