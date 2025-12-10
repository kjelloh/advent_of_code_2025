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
  return {sv,sv,sv};
}

std::vector<std::string_view> to_buttons_svs(std::string_view sv) {
  return {};
}

Lights to_lights(std::string_view sv) {
  return {};
}

Button to_button(std::string_view sv) {
  return {};
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
  return {};
}

Machine to_machine(std::string_view sv) {
  auto const& [lights,buttons,joltage] = to_sections_svs(sv);
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
