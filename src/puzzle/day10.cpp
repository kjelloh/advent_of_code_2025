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

using INT = int64_t;
using UINT = uint64_t;

using Lights = std::string;
using Button = std::vector<unsigned>;
using Buttons = std::vector<Button>;
using Joltage = std::vector<unsigned>;

struct Machine {
  Lights lights;
  std::vector<Button> buttons;
  Joltage joltage;

};

std::tuple<std::string_view,std::string_view,std::string_view> to_sections_svs(std::string_view sv) {
  // [.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
  //       ^                               ^
  auto pos1 = sv.find(']')+1;
  auto pos2 = sv.find('{');  
  return {
    sv.substr(0,pos1)
    ,sv.substr(pos1,pos2-pos1 -1)
    ,sv.substr(pos2)};  
}

std::vector<std::string_view> to_buttons_svs(std::string_view sv) {
  // ' (3) (1,3) (2) (2,3) (0,2) (0,1)'
  //      ^     ^   ^     ^     ^
  std::vector<std::string_view> result{};
  size_t begin{0};
  aoc::print("\nto_buttons_svs:");
  size_t pos{0};
  while (pos < sv.size()) {
    auto end = sv.find(')',pos);
    result.push_back(sv.substr(pos,end-pos+1));
    aoc::print(" '{}'",result.back());
    pos = end+1;
  }
  return result;
}

Lights to_lights(std::string_view sv) {
  // [.##.]
  return Lights{sv.substr(1,sv.size()-2)};
}

Button to_button(std::string_view sv) {
  // ' (1,3)'
  // ' (3)'
  Button result{};
  aoc::print("\nto_button:");
  auto begin = sv.find('(') + 1;
  auto find_end = [&sv](size_t begin){
    auto pos = sv.find(',',begin);
    if (pos == std::string_view::npos) pos = sv.find(')',begin);
    return pos;
  };
  auto end = find_end(begin);
  while (begin < sv.size()) {
    try {
      std::string val(sv.substr(begin,end-begin));
      aoc::print(" '{}'",val);
      result.push_back(std::stoi(val));
    }
    catch (std::exception const& e) {
      std::print("\nto_button: Excpetion - {}",e.what());
    }
    begin = end+1;
    end = find_end(begin);
  }
  return result;
}

Buttons to_buttons(std::string_view sv) {
  Buttons result{};
  auto svs = to_buttons_svs(sv);
  for (auto const& sv : svs) {
    result.push_back(to_button(sv));
  }
  return result;
}

Joltage to_joltage(std::string_view sv) {
  Joltage result{};
  aoc::print("\nto_joltage:");
  auto begin = sv.find('{') + 1;
  auto find_end = [&sv](size_t begin){
    auto pos = sv.find(',',begin);
    if (pos == std::string_view::npos) pos = sv.find('}',begin);
    return pos;
  };
  auto end = find_end(begin);
  while (begin < sv.size()) {
    try {
      std::string val(sv.substr(begin,end-begin));
      aoc::print(" '{}'",val);
      result.push_back(std::stoi(val));
    }
    catch (std::exception const& e) {
      std::print("\to_joltage: Excpetion - {}",e.what());
    }
    begin = end+1;
    end = find_end(begin);
  }
  return result;
}

Machine to_machine(std::string_view sv) {
  auto const& [lights,buttons,joltage] = to_sections_svs(sv);
  aoc::print(
    "\nto_machine '{}' '{}' '{}'"
    ,lights
    ,buttons
    ,joltage);
  return Machine {
    .lights = to_lights(lights)
    ,.buttons = to_buttons(buttons)
    ,.joltage = to_joltage(joltage)
  };
}

using Model = std::vector<Machine>;
Model parse(std::istream& in) {
  Model model{};

  std::string entry;
  int ix{0};
  while (std::getline(in, entry)) {
    aoc::print("\nin[{:4}][0..{:3}]: '{}' ", ix++,entry.size()-1,entry);
    model.push_back(to_machine(entry));
    aoc::print(
       "\nmachine: lights:'{}'"
      ,model.back().lights);
  }
  return model;
}

std::optional<std::string> p1(PuzzleArgs puzzle_args) {

  std::ifstream in{puzzle_args.in_file_path()};
  auto model = parse(in);

  // Solve here
  UINT candidate{};

  return {};
  // return std::format("Not yet fully implemented");
  // return std::format("{}",candidate);

} // p1

std::optional<std::string> p2(PuzzleArgs puzzle_args) {
  return {};
  // return std::format("Not yet fully implemented");
  // return std::format("{}",candidate);
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
