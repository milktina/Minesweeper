#cmake_minimum_required(VERSION 3.23)
#project(Project3Minesweeper)
#
#set(CMAKE_CXX_STANDARD 14)
#
#add_executable(Project3Minesweeper main.cpp)

cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Project3MinesweeperNew2)

## If you want to link SFML statically
# set(SFML_STATIC_LIBRARIES TRUE)

## In most cases better set in the CMake cache
# set(SFML_DIR "<sfml root prefix>/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)
add_executable(Minesweeper main.cpp TextureManager.cpp Random.cpp)
target_link_libraries(Minesweeper sfml-system sfml-window sfml-graphics)
