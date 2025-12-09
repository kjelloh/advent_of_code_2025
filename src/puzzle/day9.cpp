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

class Tile {
public:
  INT x;
  INT y;
  Tile() {}
  Tile(std::string const& s) {
    std::istringstream iss{s};
    std::string number;
    std::getline(iss,number,',');
    x = std::stoll(number);
    std::getline(iss,number,',');
    y = std::stoll(number);    
  }
};

std::string to_string(Tile const& tile) {
  return std::format("{},{}",tile.x,tile.y);
}

struct Frame {
  Tile t1;
  Tile t2;
};

struct FrameWithArea {
  Frame frame{};
  UINT area{};
};

using Model = std::vector<Tile>;

Model parse(std::istream& in) {
  Model model{};

  std::string entry;
  int ix{0};
  while (std::getline(in, entry)) {
    aoc::print("\nin[{:4}][0..{:3}]: '{}' ", ix++,entry.size()-1,entry);
    model.push_back(Tile(entry));
  }
  return model;
}

UINT spanned_area(Tile t1,Tile t2) {
  return ((std::abs(t2.x-t1.x) +1)* (std::abs(t2.y-t1.y) +1));
}

std::optional<std::string> p1(PuzzleArgs puzzle_args) {

  std::ifstream in{puzzle_args.in_file_path()};
  auto model = parse(in);

  // Solve here
  UINT V = model.size();
  UINT candidate{};

  std::vector<FrameWithArea> areaed_frames(V*(V-1)/2);
  unsigned ix{};
  for (int i=0;i<V;++i) {
    for (int j=i+1;j<V;++j) {
      auto frame = Frame{model[i],model[j]};
      auto area = spanned_area(frame.t1,frame.t2);
      areaed_frames[ix] = FrameWithArea(frame,area);
      ++ix;
    }
  }

  ix = 0;
  for (auto const& [frame,area] : areaed_frames) {
    aoc::print(
       "\n{}: {} x {} = {}"
      ,ix++ 
      ,to_string(frame.t1)
      ,to_string(frame.t2)
      ,area);
  }

  auto iter = std::ranges::max_element(areaed_frames,[](auto const& lhs,auto const& rhs){
    return lhs.area < rhs.area;
  });

  UINT max{};
  for (auto const& [frame,area] : areaed_frames) {
    max = std::max(max,area);
  }

  aoc::print("\nmax:{}",max);

  aoc::print(
     "\nmax:{} for {} - {}"
    ,iter->area
    ,to_string(iter->frame.t1)
    ,to_string(iter->frame.t2));

  // max:4762764240 for 84968,83848 - 15781,15007
  // (15781-84968 + 1) * (15007-83848 +1)
  // -69186 * -68840 = 4 762 764 240 (order of 10^9)
  // 2^64 = 1 844 674 407 370 955 xxx (order of 10^19)
  // 2^32 = 4 294 967 296 (order of 10^9)

  candidate = iter->area; // 4762902267,4762764240 not correct
                          //            4763040296

  // return {};
  // return std::format("Not yet fully implemented");
  return std::format("{}",candidate);

} // p1

std::optional<std::string> p2(PuzzleArgs puzzle_args) {

  std::ifstream in{puzzle_args.in_file_path()};
  auto model = parse(in);

  // Solve here
  const UINT V = model.size();
  UINT candidate{};

  // Compress ranges to compressed matrix
  std::vector<INT> xs(V);
  std::vector<INT> ys(V);
  for (size_t i=0;i<V;++i) {
    xs[i] = model[i].x;
    ys[i] = model[i].y;
  }

  std::ranges::sort(xs);
  std::ranges::sort(ys);

  const std::pair<INT,INT> X_BOUND(xs.front()-1,xs.back()+1);
  const std::pair<INT,INT> Y_BOUND(ys.front()-1,ys.back()+1);

  std::vector<std::pair<INT,INT>> compressed_x(V+1);
  compressed_x[0] = std::make_pair(X_BOUND.first,xs[0]);
  compressed_x[V] = std::make_pair(xs[V-1],X_BOUND.second);
  std::vector<std::pair<INT,INT>> compressed_y(V+1);
  compressed_y[0] = std::make_pair(Y_BOUND.first,ys[0]);
  compressed_y[V] = std::make_pair(ys[V-1],Y_BOUND.second);
  for (size_t i=1;i<V;++i) {
    compressed_x[i] = std::make_pair(compressed_x[i-1].second,xs[i]);
    compressed_y[i] = std::make_pair(compressed_y[i-1].second,ys[i]);
  }

  std::vector<std::vector<char>> compressed(
     V+1
    ,std::vector<char>(V+1)
  );



  // return {};
  return std::format("Not yet fully implemented");
  // return std::format("{}",candidate);
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
