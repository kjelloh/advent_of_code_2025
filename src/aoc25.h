#pragma once

#include <vector>
#include <string>


#ifdef _WIN32
  #define AOC25_EXPORT __declspec(dllexport)
#else
  #define AOC25_EXPORT
#endif

AOC25_EXPORT void aoc25();
AOC25_EXPORT void aoc25_print_vector(const std::vector<std::string> &strings);
