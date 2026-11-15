cmake_minimum_required(VERSION 3.16)

project(ProjetCpp CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(ProjetCpp
    main.cpp
    Room.cpp
    Event.cpp
    Screening.cpp
    Concert.cpp
    Play.cpp
    ScheduledEvent.cpp
)

target_compile_options(ProjetCpp PRIVATE -Wall -Wextra)