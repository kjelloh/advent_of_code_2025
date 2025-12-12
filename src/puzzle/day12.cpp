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
#include <array>

using INT = int64_t;
using UINT = uint64_t;

const unsigned W = 3;
const unsigned L = 3;
const unsigned N = 6;

using Shape = std::array<std::array<char,W>,L>;
struct Size {
  unsigned w;
  unsigned l;
};
using Quantaties = std::array<unsigned,N>;
struct Region {
  Size s;
  Quantaties q;
};
struct Model {
  std::array<Shape,N> shapes;
  std::vector<Region> regions;
};

Model parse(std::istream& in) {
  Model model{};

  int state{0};
  int sx{};
  int ix{};
  std::string entry;
  int lx{0};
  while (std::getline(in, entry)) {
    aoc::print("\nin[{:4}][{:3}]: '{}' ", lx++,entry.size(),entry);

    bool processed{};
    while (!processed) {
      processed = false;;
      switch (state) {
        case 0: {
          ix = 0;
          if (entry.find('x') == std::string::npos) {
            sx = entry[0] - '0';
            state = 1;
            processed = true;
          }
          else {
            state = 2;
          }
        } break;
        case 1: {
          if (entry.size()==0) {
            state = 0;
          }
          else {
            aoc::print(" shape {}[{}]",sx,ix);
            for (size_t i=0;i<W;++i) {
              model.shapes[sx][ix][i] = entry[i];
            }
            ++ix;
          }
          processed = true;
        } break;
        case 2: {
          aoc::print(" region");
          auto split = entry.find('x');
          auto end = entry.find(':',split);
          Size size{
             .w = static_cast<unsigned>(std::stoi(entry.substr(0,split)))
            ,.l = static_cast<unsigned>(std::stoi(entry.substr(split+1,end)))};
          aoc::print(" {}x{}",size.w,size.l);

          std::istringstream iss(entry.substr(end+2));
          unsigned count;
          int i = 0;
          Quantaties q;
          while (iss >> count) {
            q[i] = count;
            aoc::print(" q[{}]={}",i,q[i]);
            ++i;
          }
          model.regions.push_back(Region{
             .s = size
            ,.q = q
          });
          aoc::print(" {}",model.regions.size());
          processed = true;
        } break;
      } // switch state
    } // while !processed
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
    "\nday part:{} debug:{}"
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
