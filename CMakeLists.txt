cmake_minimum_required(VERSION 3.17.0)

project(elevate_game)
include_directories("include")

set(CMAKE_CXX_STANDARD 20)

Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://gitclone.com/github.com/catchorg/Catch2
        GIT_TAG        v3.0.0-preview3
)

FetchContent_MakeAvailable(Catch2)

add_executable(elevate_game main.cpp)

target_link_libraries(elevate_game PRIVATE Catch2::Catch2WithMain)