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
#include <format>

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

// Specialize std::formatter for Shape
template<>
struct std::formatter<Shape> {

    template<class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx) {
      return ctx.begin();
    }

    template<class FmtContext>
    FmtContext::iterator format(Shape  shape, FmtContext& ctx) const {
        auto out = ctx.out();        
        for (std::size_t i = 0; i < L; ++i) {
            if (i>0) std::format_to(out,"\n");
            for (std::size_t j = 0; j < W; ++j) {
              std::format_to(out,"{}",shape[i][j]);
            }
        }
        return out;
    }
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
          // aoc::print(" {}",model.regions.size());
          processed = true;
        } break;
      } // switch state
    } // while !processed
  }
  return model;
}

std::optional<std::string> test_p1(Model const& model,int i,int test_ix) {
    aoc::print("\ntest_p1 {}:{}",i,test_ix);
    if (test_ix > 0 and i==0) switch (test_ix) {
        return "TEST!";

      case 1: {
        // 4x4: 0 0 0 0 2 0

        // The first region is 4x4:

        // ....
        // ....
        // ....
        // ....
        // In it, you need to determine whether you could fit two presents that have shape index 4:

        // ###
        // #..
        // ###
        // After some experimentation, it turns out that you can fit both presents in this region. Here is one way to do it, using A to represent one present and B to represent the other:

        // AAA.
        // ABAB
        // ABAB
        // .BBB
        auto [w,l] = model.regions[i].s; 
        if (w != 4 or l != 4) {
          return std::format("Expected {}x{} to be 4x4",w,l);
        }
        std::print("\n{}",model.shapes[4]);

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

std::optional<std::string> test_p2(Model const& model, int i,int test_ix) {
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

  for (size_t i=0;i<model.regions.size();++i) {
    aoc::print("\nSolve for region:{}",i);
    if (for_part2) {
      if (auto test_result = test_p2(model,i,test_ix)) {
        return *test_result;
      }
    }
    else {
      if (auto test_result = test_p1(model, i,test_ix)) {
        return *test_result;
      }
    }
  }

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
