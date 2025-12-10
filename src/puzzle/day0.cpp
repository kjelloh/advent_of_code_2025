#include "aoc25.h"
#include <string>
#include <print>
#include <iostream>
#include <fstream>
#include <optional>
#include <algorithm>
#include <set>
#include <map>
#include <sstream>
#include <unordered_set>

using INT = int64_t;
using UINT = uint64_t;

using Entry = std::string;
using Model = std::vector<Entry>;
Model parse(std::istream& in) {
  Model model{};

  std::string entry;
  int ix{0};
  while (std::getline(in, entry)) {
    aoc::print("\nin[{:4}][0..{:3}]: '{}' ", ix++,entry.size()-1,entry);
    model.push_back(Entry(entry));
  }
  return model;
}

std::optional<std::string> test_p1(int i,int test_ix) {
    if (test_ix > 0 and i==0) switch (test_ix) {
      case 1: {
      } break;
    }

    if (test_ix > 0 and i==1) switch (test_ix) {
      case 2: {
      } break;
    };

    if (test_ix > 0 and i==2) switch (test_ix) {
      case 3: {
      } break;
    };

  return {};    
}

std::optional<std::string> test_p2(int i,int test_ix) {
    if (test_ix > 0 and i==0) switch (test_ix) {
      case 1: {
      } break;
    }

    if (test_ix > 0 and i==1) switch (test_ix) {
      case 2: {
      } break;
    };

    if (test_ix > 0 and i==2) switch (test_ix) {
      case 3: {
      } break;
    };

  return {};    
}

std::optional<std::string> solve(PuzzleArgs puzzle_args,bool for_part2 = false) {

  auto test_ix = puzzle_args.meta().m_maybe_test.value_or(0);

  std::ifstream in{puzzle_args.in_file_path()};
  auto model = parse(in);

  // Solve here
  UINT candidate{};

  return {};
  // return std::format("Not yet fully implemented");
  // return std::format("{}",candidate);

} // solve


std::optional<std::string> p1(PuzzleArgs puzzle_args) {
  return solve(puzzle_args,false); 
} // p1

std::optional<std::string> p2(PuzzleArgs puzzle_args) {
  return solve(puzzle_args,true);
} // p2

std::optional<std::string> day(PuzzleArgs puzzle_args) {
  aoc::print(
    "\nday10 part:{} debug:{}"
    ,puzzle_args.meta().m_part
    ,puzzle_args.meta().m_debug);
  
  switch (puzzle_args.meta().m_part) {
    case 1: {
      return p1(puzzle_args);
    } break;
    case 2:
      return p2(puzzle_args);
      break;
  }
  return std::nullopt;
}
