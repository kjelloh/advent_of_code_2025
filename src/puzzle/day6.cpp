#include "aoc25.h"
#include <string>
#include <print>
#include <iostream>
#include <fstream>
#include <optional>
#include <algorithm>
#include <sstream>

// size 5 x 3768
struct Expression {
  std::vector<size_t> operands{};
  char op{};
  size_t eval() const {
    switch (op) {
      case '+': return 0;
      case '*': return 0;
      default: return 0;
    }
  }
};

using Model = std::vector<Expression>;

Model parse(std::istream& in) {
  Model model{};
  std::string entry;
  while (std::getline(in,entry)) {
    std::print("\n{} ",entry);

    // read csv
    std::istringstream iss{entry};
    std::vector<std::string> csvs{};
    std::string s{};
    while (std::getline(iss,s,' ')) {
      csvs.push_back(s);
    }

    // entry is either the next operands or the operators
    if (csvs[0][0] >= '0' or csvs[0][0] <= '9') {
      // operands
      std::print("\noperands:{}",csvs.size());
    }
    else {
      // operators
      std::print("\noperagtors:{}",csvs.size());
    }
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
