#pragma once

#include <vector>
#include <string>


#ifdef _WIN32
  #define AOC25_EXPORT __declspec(dllexport)
#else
  #define AOC25_EXPORT
#endif

int solve();

AOC25_EXPORT void print_platform();
