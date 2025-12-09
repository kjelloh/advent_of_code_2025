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
    x = std::stoi(number);
    std::getline(iss,number,',');
    y = std::stoi(number);    
  }
};

struct Frame {
  Tile t1;
  Tile t2;
};

struct FrameWithArea {
  Frame frame{};
  UINT area{};
};

std::string to_string(Tile const& tile) {
  return std::format("{},{}",tile.x,tile.y);
}

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
  return (std::abs(t2.x-t1.x + 1) * std::abs(t2.y-t1.y + 1));
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
      ,to_string(frame.t1)
      ,area);
  }

  auto iter = std::ranges::max_element(areaed_frames,[](auto const& lhs,auto const& rhs){
    return lhs.area < rhs.area;
  });

  aoc::print(
     "\nmax:{} for {} - {}"
    ,iter->area
    ,to_string(iter->frame.t1)
    ,to_string(iter->frame.t2));

  // max:4762764240 for 84968,83848 - 15781,15007
  // (15781-84968 + 1) * (15007-83848 +1)
  // -69186 * -68840 = 4762764240

  candidate = iter->area; // 4762902267,4762764240 not correct

  // return {};
  // return std::format("Not yet fully implemented");
  return std::format("{}",candidate);

} // p1

std::optional<std::string> p2(PuzzleArgs puzzle_args) {
  return {};
  // return std::format("Not yet fully implemented");
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
