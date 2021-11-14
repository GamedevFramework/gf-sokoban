cmake_minimum_required(VERSION 3.10)

project(SOKOBAN02
  VERSION 0.1.0
  LANGUAGES CXX
)

include(GNUInstallDirs)

find_package(gf REQUIRED COMPONENTS graphics)

set(SOKOBAN_DATA_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATAROOTDIR}/games/sokoban")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/code/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

add_executable(sokoban02
  code/sokoban02.cc
  code/bits/Boxes.cc
  code/bits/Hero.cc
  code/bits/Level.cc
)

target_include_directories(sokoban02
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(sokoban02
  PRIVATE
    gf::graphics
)

install(
  TARGETS sokoban02
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data/sokoban"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/games"
)

