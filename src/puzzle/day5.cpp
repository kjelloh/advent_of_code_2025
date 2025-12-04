#include "aoc25.h"
#include <string>
#include <print>
#include <iostream>
#include <fstream>
#include <optional>

std::optional<size_t> p1(PuzzleArgs puzzle_args) {
  std::optional<size_t> answer{};
  std::ifstream in{puzzle_args.in_file_path()};
  std::string entry;
  while (std::getline(in,entry)) {
    std::print("\n{} ",entry);
  }

  return answer;
}

std::optional<size_t> p2(PuzzleArgs puzzle_args) {
  std::optional<size_t> answer{};
  std::ifstream in{puzzle_args.in_file_path()};
  std::string entry;
  while (std::getline(in,entry)) {
    std::print("\n{} ",entry);
  }

  return answer;
}

std::string day(PuzzleArgs puzzle_args) {
  std::print("\nday05");
  std::string result{};
  if (auto answer = p1(puzzle_args)) {
    result += std::format("p1:{}",*answer);
  }
  if (auto answer = p2(puzzle_args)) {
    result += std::format(" p2:{}",*answer);
  }
  return result;
}
