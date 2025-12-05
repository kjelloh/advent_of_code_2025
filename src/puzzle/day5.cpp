#include "aoc25.h"
#include <string>
#include <print>
#include <iostream>
#include <fstream>
#include <optional>
#include <algorithm>

// 334 442 869 995 852 = 3.34.. * 10^15
// 2^64 = 1.8.. * 10^19

std::optional<size_t> p1(PuzzleArgs puzzle_args) {
  std::optional<size_t> answer{};
  std::ifstream in{puzzle_args.in_file_path()};
  std::string entry;
  bool is_ranges{true};
  using Model = std::pair<
     std::vector<std::pair<size_t,size_t>>
    ,std::vector<size_t>>;
  Model model{};
  while (std::getline(in,entry)) {
    std::print("\n{} ",entry);
    if (entry.size() == 0) {
      is_ranges = false;
      continue;
    }
    if (is_ranges) {
      auto pos = entry.find('-');
      model.first.push_back({
         std::stoll(entry.substr(0,pos))
        ,std::stoll(entry.substr(pos+1))
      });
    }
    else {
      model.second.push_back(std::stoll(entry));
    }

  }

  size_t acc{};
  for (auto id : model.second) {
    std::string log{};
    std::print("\nIngridient {}",id);
    auto is_fresh = std::ranges::any_of(model.first,[id,&log](auto range){
      auto result = (range.first <= id) and (id <= range.second);
      if (result) {
        log = std::format(
           "falls into range {}-{}"
          ,range.first
          ,range.second);
      }
      return result;
    });
    if (is_fresh) {
      std::print(" is fresh because it {}",log);
      ++acc;
    }
    else {
      std::print(" is spoiled because it does not fall into any range");
    }
  }
  answer = acc;

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

std::optional<std::string> day(PuzzleArgs puzzle_args) {
  std::print("\nday05 :)");
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
