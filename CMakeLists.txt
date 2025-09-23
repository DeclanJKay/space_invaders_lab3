cmake_minimum_required(VERSION 3.11)
project(Pong)

# Require modern C++
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#### Setup Directories ####
# Main output directory
set(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
# Output all DLLs from all libs into main build folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

#### Add External Dependencies ####
add_subdirectory("lib/SFML")
set(SFML_INCS "lib/SFML/include")

#### Pong Project ####
add_executable(Pong main.cpp)

target_include_directories(Pong PRIVATE ${SFML_INCS})

target_link_libraries(Pong 
    sfml-graphics 
    sfml-window 
    sfml-system
)
