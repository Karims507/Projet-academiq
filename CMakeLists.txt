cmake_minimum_required(VERSION 3.16)
project(Projet-academiq LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Projet-academiq
    src/main.cpp
    src/Date.cpp
    src/Event.cpp
    src/Venue.cpp
    src/Util.cpp
    src/Storage.cpp
    src/Console.cpp
    src/Screening.cpp
    src/Concert.cpp
    src/Play.cpp
)

target_compile_options(Projet-academiq PRIVATE -Wall -Wextra)