# Minimum CMake version required
cmake_minimum_required(VERSION 3.26)

# Project name and version
project(centicrawler LANGUAGES CXX VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add the executable
add_executable(${PROJECT_NAME}
                src/main.cpp
                src/AnimatingSprite.cpp
                src/Engine.cpp
                src/SoundManager.cpp
                src/TextureManager.cpp
                src/Player.cpp
                src/Laser.cpp
                src/Mushrooms.cpp
                src/Ant.cpp
                src/Scorpion.cpp
                src/Spider.cpp
                src/Centipede.cpp
                src/gfx/Drawable.cpp
                src/gfx/Input.cpp
                src/gfx/RectangleShape.cpp
                src/gfx/RenderWindow.cpp
                src/gfx/Sprite.cpp
                src/gfx/Texture.cpp)

INCLUDE(FindPkgConfig)

#Find the required libraries.
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2_IMAGE REQUIRED SDL2_image)
PKG_SEARCH_MODULE(SDL2_MIXER REQUIRED SDL2_mixer)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_MIXER_LIBRARIES}
        ${SDL2MAIN_LIBRARIES}
)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
# Enable warning and errors
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wsign-conversion -Wconversion -Wextra -Werror -pedantic -pedantic-errors)

# ensure assets are copied to build directory
# need a better solution in the code
# to solve paths relative to cwd problem
file(COPY ${PROJECT_SOURCE_DIR}/graphics
     DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
file(COPY ${PROJECT_SOURCE_DIR}/sounds
     DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
