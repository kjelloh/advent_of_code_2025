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
    size_t result{};
    switch (op) {
      case '+': return std::ranges::fold_left(operands, size_t{0}, std::plus<>{});
      case '*': return std::ranges::fold_left(operands, size_t{1}, std::multiplies<>{});;
      default: return 0;
    }
  }

  std::string to_string() const {
    std::string result{};
    for (auto const& op : operands) result += (std::to_string(op) + " ");
    result.push_back(op);
    return result;
  }

};

using Model = std::vector<Expression>;

Model parse1(std::istream& in) {
  Model model{};

  std::string entry;
  while (std::getline(in, entry)) {
    std::print("\n{} ", entry);

    std::istringstream iss(entry);
    std::vector<std::string> csvs;
    std::string s;

    while (iss >> s) {
      csvs.push_back(s);
    }

    if (std::isdigit(static_cast<unsigned char>(csvs[0][0]))) {
      std::print("\noperands: {}", csvs.size());
      for (int i=0;i<csvs.size();++i) {
        // i is expression index
        if (model.size() < i+1) model.push_back({});
        model[i].operands.push_back(std::stoll(csvs[i]));
        std::print(" {}:{}",i,csvs[i]);
      }
    } else {
      std::print("\noperators: {}", csvs.size());
      for (int i=0;i<csvs.size();++i) {
        // i is expression index
        if (model.size() < i+1) model.push_back({});
        model[i].op = csvs[i][0];
        std::print(" {}:{}",i,csvs[i][0]);
      }
    }
  }
  return model;
}

std::optional<size_t> p1(PuzzleArgs puzzle_args) {
  std::optional<size_t> answer{};
  std::ifstream in{puzzle_args.in_file_path()};
  auto model = parse1(in);
  // Solve here
  size_t acc{};
  for (auto const& expression : model) {
    auto val = expression.eval();
    acc += val;
    std::print("\n{} = {}",expression.to_string(),val);
  }
  answer = acc;
  return answer;
}

Model parse2(std::istream& in) {
  Model model{};

  std::vector<std::string> grid{};
  std::string ops{};

  std::string entry;
  while (std::getline(in, entry)) {
    std::print("\n{} ", entry);
    // split on each single space following text
    int state{0};
    std::vector<std::string> tokens{};
    tokens.push_back(""); // expect at least one token
    int i{0};
    // '123 328  51 64 '
    while (i < static_cast<int>(entry.size())) {
      char ch = entry[i];
      // '1' '2' '3' ' '
      // '3' '2' '8' ' '
      // ' ' '5' '1' ' '
      // '6' '4' ' '
      switch (state) {
        case 0:
          // Read leading spaces
          if (ch != ' ') {
            // '1'
            // '3'
            // '5'
            // '6'
            state = 1;
            continue;
          }
          // ' '
          tokens.back().push_back(ch);
          ++i; // next
          break;
        case 1:
          // Read text
          if (ch == ' ') {
            // ' '
            // ' '
            // ' '
            // ' '
            // New token if more input
            if (i < entry.size()) tokens.push_back(""); // '123' '328' ' 51' '64'
            ++i; // eat separator ' '
            state = 0;
            continue;
          }

          // '1' '2' '3'
          // '3' '2' '8'
          // '5' '1'
          // '6' '4'
          tokens.back().push_back(ch);
          ++i; // next

          break;        
      }
    } // while chars -> tokens

    for (auto const& token : tokens) {
      std::print("\n    '{}'",token);
    }

  }

  return model;
}

std::optional<size_t> p2(PuzzleArgs puzzle_args) {
  std::optional<size_t> answer{};
  std::ifstream in{puzzle_args.in_file_path()};
  auto model = parse2(in);
  // Solve here
  return answer;
}

std::optional<std::string> day(PuzzleArgs puzzle_args) {
  std::print(
    "\nday6 part:{} debug:{}"
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
