#include "aoc25.h"
#include <string>
#include <print>
#include <iostream>
#include <fstream>
#include <optional>
#include <algorithm>
#include <sstream>

using Model = std::vector<std::string>;
Model parse(std::istream& in) {
  Model model{};

  std::string entry;
  int ix{0};
  while (std::getline(in, entry)) {
    std::print("\n{}[0..{}]:'{}' ", ix++,entry.size()-1,entry);
    model.push_back(entry);
  }
  return model;
}

std::optional<size_t> p1(PuzzleArgs puzzle_args) {
  std::optional<size_t> answer{};
  std::ifstream in{puzzle_args.in_file_path()};
  auto model = parse(in);
  // Solve here
  size_t acc{};
  // answer = acc;
  return answer;
}

std::optional<size_t> p2(PuzzleArgs puzzle_args) {
  std::optional<size_t> answer{};
  std::ifstream in{puzzle_args.in_file_path()};
  auto model = parse(in);
  // Solve here
  size_t acc{};
  // answer = acc;
  return answer;
}

std::optional<std::string> day(PuzzleArgs puzzle_args) {
  std::print(
    "\nday part:{} debug:{}"
    ,puzzle_args.meta().m_part
    ,puzzle_args.meta().m_debug);
  
  switch (puzzle_args.meta().m_part) {
    case 1: {
      return p1(puzzle_args)
        .transform([](auto answer){
          return std::format("{}",answer);
        });
    } break;
    case 2:
      return p2(puzzle_args)
        .transform([](auto answer){
          return std::format("{}",answer);
        });
      break;
  }
  return std::nullopt;
}
