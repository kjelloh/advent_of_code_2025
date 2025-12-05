#include "aoc25.h"
#include <string>
#include <print>
#include <iostream>
#include <fstream>
#include <optional>
#include <algorithm>

using Model = std::vector<std::string>;
Model parse(std::istream& in) {
  Model model{};
  std::string entry;
  bool is_ranges{true};
  while (std::getline(in,entry)) {
    std::print("\n{} ",entry);
    model.push_back(entry);
  }
  return model;
}

std::optional<size_t> p1(PuzzleArgs puzzle_args) {
  std::optional<size_t> answer{};
  std::ifstream in{puzzle_args.in_file_path()};
  auto model = parse(in);
  // Solve here
  return answer;
}

std::optional<size_t> p2(PuzzleArgs puzzle_args) {
  std::optional<size_t> answer{};
  std::ifstream in{puzzle_args.in_file_path()};
  auto model = parse(in);
  // Solve here
  return answer;
}

std::optional<std::string> day(PuzzleArgs puzzle_args) {
  std::print(
    "\nday6 part:{} debug:{}"
    ,puzzle_args.meta().m_part
    ,puzzle_args.meta().m_debug);
  std::string result{};
  auto answer1 = p1(puzzle_args);
  if (answer1) {
    result += std::format("p1:{}",*answer1);
    auto answer2 = p2(puzzle_args);
    if (answer2) {
      result += std::format("p1:{}",*answer2);      
    }
  }

  if (result.size() > 0) return result;
  return std::nullopt;
}
