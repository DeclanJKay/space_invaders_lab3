cmake_minimum_required(VERSION 3.11)
project(space_invaders_lab3)

# Require modern C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#### Setup Directories ####
# Main output directory (bin)
set(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

#### Add External Dependencies ####
add_subdirectory("lib/SFML")
set(SFML_INCS "lib/SFML/include")

#### Space Invaders Lab 3 Project ####
add_executable(space_invaders_lab3 
main.cpp 
Ship.cpp 
game_systems.cpp)

target_include_directories(space_invaders_lab3 PRIVATE ${SFML_INCS})

target_link_libraries(space_invaders_lab3 
    sfml-graphics 
    sfml-window 
    sfml-system
)

#### Copy Resources ####
# Copy resources into both bin/res (for running manually)
# AND bin/Debug/res (for Visual Studio debug mode)
add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/res"
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/res"
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/res
)

add_dependencies(space_invaders_lab3 copy_resources)

#### Set working directory ####
# Visual Studio runs from bin/Debug when debugging
# so assets are always found automatically
set_target_properties(space_invaders_lab3
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>
)
