cmake_minimum_required(VERSION 3.25)

project(2048
    VERSION 0.1.0
    LANGUAGES CXX
)

# =========================================================
# C++ configuration
# =========================================================

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Static libraries
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)


# =========================================================
# Dependencies
# =========================================================

include(FetchContent)

# ---------------------------------------------------------
# raylib
# ---------------------------------------------------------

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 6.0
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(raylib)


# ---------------------------------------------------------
# EnTT
# ---------------------------------------------------------

FetchContent_Declare(
    entt
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG v4.0.0
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(entt)


# =========================================================
# Game
# =========================================================

file(GLOB_RECURSE GAME_SOURCES
    CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(${PROJECT_NAME}
    ${GAME_SOURCES}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        raylib
        EnTT::EnTT
)


# =========================================================
# Compiler warnings
# =========================================================

if (MSVC)

    target_compile_options(${PROJECT_NAME}
        PRIVATE
            /W4
            /permissive-
    )

else()

    target_compile_options(${PROJECT_NAME}
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )

endif()


# =========================================================
# Assets
# =========================================================

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)