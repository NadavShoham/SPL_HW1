cmake_minimum_required(VERSION 3.20)
project(SPL_HW1)
include_directories(SYSTEM ./include)
set(CMAKE_CXX_STANDARD 14
        src/main.cpp
        src/Studio.cpp
        src/Trainer.cpp
        src/Workout.cpp
        src/Customer.cpp)

add_executable(SPL_HW1 src/main.cpp src/Studio.cpp src/Trainer.cpp src/Workout.cpp src/Customer.cpp src/Action.cpp src/Action.h)
