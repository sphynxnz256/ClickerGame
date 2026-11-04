cmake_minimum_required(VERSION 3.28)
project(ClickerGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Download and build SFML from source on the first configure.
# It's built as static libraries so the game doesn't need the SFML DLLs.
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SFML)

add_executable(ClickerGame
    src/main.cpp
    src/core/Game.cpp
    src/core/Level.cpp
    src/core/RNG.cpp
    src/entities/Boss.cpp
    src/entities/Coin.cpp
    src/entities/Enemy.cpp
    src/entities/Player.cpp
    src/graphics/Animator.cpp
    src/ui/Button.cpp
    src/ui/UI.cpp
)

target_precompile_headers(ClickerGame PRIVATE src/stdafx.h)
if(MSVC)
    target_compile_options(ClickerGame PRIVATE /W3 /sdl)
endif()
target_include_directories(ClickerGame PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(ClickerGame PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Network
    SFML::Audio
)

# Copy the game assets next to the executable so it runs on its own.
add_custom_command(TARGET ClickerGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:ClickerGame>
)
