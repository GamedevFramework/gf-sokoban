cmake_minimum_required(VERSION 3.10)

project(SOKOBAN01
  VERSION 0.1.0
  LANGUAGES CXX
)

find_package(gf REQUIRED COMPONENTS graphics)

add_executable(sokoban01
  code/sokoban01.cc
)

target_link_libraries(sokoban01
  PRIVATE
    gf::graphics
)
