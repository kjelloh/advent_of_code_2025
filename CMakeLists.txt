cmake_minimum_required(VERSION 3.15)
project(aoc25 CXX)

# Read the current day from workspace/day.txt, default to 0 if file doesn't exist
if(EXISTS "${CMAKE_SOURCE_DIR}/workspace/day.txt")
    file(STRINGS "${CMAKE_SOURCE_DIR}/workspace/day.txt" DAY_NUMBER LIMIT_COUNT 1)
    string(STRIP "${DAY_NUMBER}" DAY_NUMBER)
else()
    message(WARNING "workspace/day.txt not found - falling back to day 0")
    set(DAY_NUMBER "0")
endif()

# Set the day file path (no leading zero)
set(DAY_FILE "src/puzzle/day${DAY_NUMBER}.cpp")

# Check if the file exists, fallback to day0.cpp if not
if(EXISTS "${CMAKE_SOURCE_DIR}/${DAY_FILE}")
    message(STATUS "Building for Day ${DAY_NUMBER} using file: ${DAY_FILE}")
else()
    message(WARNING "Day file not found: ${DAY_FILE} - falling back to day0.cpp")
    set(DAY_FILE "src/puzzle/day0.cpp")
endif()

add_executable(aoc25
  ${DAY_FILE}
  src/aoc25.cpp
  src/main.cpp)

install(TARGETS aoc25 DESTINATION "."
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        )
