cmake_minimum_required(VERSION 4.0)
project(groupProject)

set(CMAKE_CXX_STANDARD 11)

add_executable(groupProject
        main.cpp
        SDL_Plotter.cpp
        Racer.h
        RaceCarDriver.h
        Timer.h
        DisjointSet.h
)

target_include_directories(groupProject PRIVATE /opt/homebrew/include)
target_link_directories(groupProject PRIVATE /opt/homebrew/lib)

target_link_libraries(groupProject PRIVATE
        SDL2
        SDL2_mixer
)