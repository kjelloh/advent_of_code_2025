cmake_minimum_required(VERSION 3.15)
project(aoc25 CXX)



add_executable(aoc25 
  src/puzzle/day02.cpp
  src/puzzle/day01.cpp
  src/puzzle/day00.cpp
  src/aoc25.cpp 
  src/main.cpp)

install(TARGETS aoc25 DESTINATION "."
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )
