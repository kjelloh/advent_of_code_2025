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

  std::vector<std::string> entries{};
  std::string entry;
  std::string sum_string{};
  while (std::getline(in, entry)) {
    std::print("\n{} ", entry);
    entries.push_back(entry);

    for (int i=0;i<entry.size();++i) {
      char ch = entry[i];
      if (sum_string.size() < i + 1) sum_string.push_back(ch);
      else sum_string[i] = std::max(sum_string[i],ch); // digits win over ' '
    }
  }

  std::print("\nsum_string:'{}'",sum_string);

  // Detect column separators
  std::vector<size_t> seps{};
  size_t i=0;
  for (int i = 0; i < static_cast<int>(sum_string.size());++i) {
    if (sum_string[i] == ' ') {
      if (seps.size() > 0 && seps.back() == i-1) {
        std::print("\nAdjacent space separators detected at {}",i);
      }
      seps.push_back(i);
    }
  }

  // Calculate column widths
  std::vector<size_t> widths{};
  int begin = 0;
  for (auto end : seps) {
    widths.push_back(end - begin);
    begin = end+1;
  }

  for (auto width : widths) {
    std::print("\n    width:{}",width);
  }

  for (auto const& entry : entries) {
    std::vector<std::string> fields{};

    // Split on column widths
    size_t begin{0};
    for (auto width : widths) {
      auto end = begin + width;
      if (end <= entry.size()) {
        fields.push_back(entry.substr(begin,width));
      }
      else {
        std::print(
           "\nFailed to consume field width:{} at {} in entry size:{}"
          ,width
          ,begin
          ,entry.size());
      }
      begin = end + 1;
    }

    for (auto const& field : fields) {
      std::print("\n    '{}'",field);
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
