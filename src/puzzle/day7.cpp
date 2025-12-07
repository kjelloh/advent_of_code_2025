#include "aoc25.h"
#include <string>
#include <print>
#include <iostream>
#include <fstream>
#include <optional>
#include <algorithm>
#include <set>
#include <map>

using Grid = std::vector<std::string>;

void print(Grid const& grid) {
  int ix{0};
  for (auto const& row : grid) {
    std::print("\n{:4}[0..{:3}]: '{}' ", ix++,row.size()-1,row);
  }
}

using Model = Grid;
Model parse(std::istream& in) {
  Model model{};

  std::string entry;
  int ix{0};
  while (std::getline(in, entry)) {
    std::print("\n{:4}[0..{:3}]: '{}' ", ix++,entry.size()-1,entry);
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
  int c = model[0].find('S');
  int dr{1};
  size_t num_rows = model.size();
  size_t num_cols = model[0].size();

  std::vector<std::set<int>> beams{
     num_rows
  };
  beams[0].insert(c);

  for (int r=1;r<model.size();++r) {
    std::vector<int> hit_splitters{};
    for (int c=0;c<num_cols;++c) {
      if (beams[r-1].contains(c)) {
        if (model[r][c] == '^') {
          hit_splitters.push_back(c);
          ++acc;
        }
        else beams[r].insert(c);
      }
    }
    for (auto splitter : hit_splitters) {
      if (splitter == 0) beams[r].insert(splitter+1);
      else if (0 < splitter and splitter < num_cols-1) {
        beams[r].insert(splitter-1);
        beams[r].insert(splitter+1);
      }
      else beams[r].insert(splitter-1);
    }
    std::print(
       "\n{} -> hit_splitters:{} beams:{}"
      ,model[r],hit_splitters.size()
      ,beams[r].size());
  }

  answer = acc;
  return answer; // 1499
}

std::optional<size_t> p2(PuzzleArgs puzzle_args) {
  std::optional<size_t> answer{};
  std::ifstream in{puzzle_args.in_file_path()};
  auto model = parse(in);
  // Solve here
  int c = model[0].find('S');
  int dr{1};
  size_t num_rows = model.size();
  size_t num_cols = model[0].size();

  std::vector<std::set<int>> beams{
     num_rows
  };
  beams[0].insert(c);

  for (int r=1;r<model.size();++r) {
    std::vector<int> hit_splitters{};
    for (int c=0;c<num_cols;++c) {
      if (beams[r-1].contains(c)) {
        if (model[r][c] == '^') {
          hit_splitters.push_back(c);
        }
        else beams[r].insert(c);
      }
    }
    for (auto splitter : hit_splitters) {
      if (splitter == 0) beams[r].insert(splitter+1);
      else if (0 < splitter and splitter < num_cols-1) {
        beams[r].insert(splitter-1);
        beams[r].insert(splitter+1);
      }
      else beams[r].insert(splitter-1);
    }
    std::print(
       "\n{} -> hit_splitters:{} beams:{}"
      ,model[r],hit_splitters.size()
      ,beams[r].size());
  }

  // Count number of possible paths to reach bottom row
  std::map<int,size_t> current{};
  for (auto beam : beams.back()) {
    current[beam] = 1;
  }
  for (int r=num_rows-1;r>0;--r) {
    std::print("\n");
    for (int c=0;c<num_cols;++c) {
      if (beams[r].contains(c)) std::print("{}",'|');
      else std::print("{}",model[r][c]);
    }
    for (auto counter : current) std::print(" {}",counter);

    std::map<int,size_t> prev{};
    for (auto beam : beams[r]) {
      if (beams[r-1].contains(beam)) prev[beam] = current[beam]; // pass on
      // attend to split
      if (beam+1 < num_cols and model[r][beam+1] == '^') prev[beam+1] += current[beam];
      if (beam-1 >= 0 and model[r][beam-1] == '^') prev[beam-1] += current[beam];        
    }
    current = prev;
  }

  if (current.size()==1) {
    answer = current.begin()->second;
  }
  else {
    std::print("\nFAILED: Expected one but found {} final counters",current.size());
  }

  return answer;
}

std::optional<std::string> day(PuzzleArgs puzzle_args) {
  std::print(
    "\nday7 part:{} debug:{}"
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
