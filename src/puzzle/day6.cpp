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
      default: return 0; // harmless for default (empty) expression
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
  std::string entry;
  while (std::getline(in, entry)) {
    std::print("\nparse2:{} ", entry);
    grid.push_back(entry);
  }

  // pivot the grid counter-clockwise on character columns
  int in_row_count = grid.size();
  int in_col_count = grid[0].size(); // fixed width

  int pivot_row_count = in_col_count;
  int pivot_col_count = in_row_count;
  std::vector<std::string> pivot_grid(
     pivot_row_count
    ,std::string(pivot_col_count,' ')
  );
  for (int r=0;r<pivot_row_count;++r) {
    for (int c=0;c<pivot_col_count;++c) {
      pivot_grid[r][c] = grid[c][in_col_count-r-1];
    }
  }

  // Now parse the pivot grid into expressions
  model.push_back({});
  for (auto entry : pivot_grid) {
    if (std::ranges::all_of(entry,[](auto ch){ return ch == ' ';})) continue;
    std::print("\npivot:'{}'",entry);
    if (entry.back() == '+' or entry.back() == '*') {
      std::print(" --> {}",entry.back());
      model.back().op = entry.back();
      entry.back() = ' ';
      model.back().operands.push_back(std::stoll(entry));
      model.push_back({}); // next expression
      continue;
    }
    model.back().operands.push_back(std::stoll(entry));
  }
  return model;
}

std::optional<size_t> p2(PuzzleArgs puzzle_args) {
  std::optional<size_t> answer{};
  std::ifstream in{puzzle_args.in_file_path()};
  auto model = parse2(in);
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
