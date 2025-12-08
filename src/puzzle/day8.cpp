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

struct Point {
  int x;
  int y;
  int z;
  Point(std::string s) {
    std::istringstream iss{s};
    std::string number;
    std::getline(iss,number,',');
    x = std::stoi(number);
    std::getline(iss,number,',');
    y = std::stoi(number);
    std::getline(iss,number,',');
    z = std::stoi(number);
  }
};

std::string to_string(Point p) {
  return std::format("[{},{},{}]",p.x,p.y,p.z);  
}

auto squared_distance(Point lhs,Point rhs) {
  auto dx = (lhs.x - rhs.x);
  auto dy = (lhs.x - rhs.x);
  auto dz = (lhs.x - rhs.x);

  return (dx*dx + dy*dy + dz*dz);
}

struct Edge {
  Point p1;
  Point p2;
  auto squared_size() const {
    return squared_distance(p1,p2);
  }
};

using Model = std::vector<Point>;
Model parse(std::istream& in) {
  Model model{};

  std::string entry;
  int ix{0};
  while (std::getline(in, entry)) {
    aoc::print("\nin[{:4}][0..{:3}]: '{}' ", ix++,entry.size()-1,entry);
    model.push_back(Point(entry));
  }
  return model;
}

std::optional<size_t> p1(PuzzleArgs puzzle_args) {
  std::optional<size_t> answer{};
  std::ifstream in{puzzle_args.in_file_path()};
  auto model = parse(in);
  // Solve here
  if (puzzle_args.meta().m_debug) for (auto p : model) {
    aoc::print("\np:{}",to_string(p));
  }

  // Brute force?
  // Yes, seems feasable for part 1
  std::vector<std::pair<Edge,int>> edges{};
  for (int i=0;i<model.size();++i) {
    for (int j=i+1;j<model.size();++j) {
      auto pi = model[i];
      auto pj = model[j];
      auto dps = squared_distance(pi,pj);
      aoc::print("\ndps:{}",dps);
      Edge edge{pi,pj};
      edges.push_back(std::make_pair(edge,edge.squared_size()));
    }
  }
  std::ranges::sort(edges,[](auto const& lhs,auto const& rhs){
    return (lhs.second < rhs.second);
  });

  for (auto const& entry : edges) {
    aoc::print(
       "\nsorted:{} ({} - {})"
      ,entry.second
      ,to_string(entry.first.p1)
      ,to_string(entry.first.p2));
  }

  size_t candidate{};
  return answer;
}

std::optional<size_t> p2(PuzzleArgs puzzle_args) {
  std::optional<size_t> answer{};
  std::ifstream in{puzzle_args.in_file_path()};
  auto model = parse(in);
  // Solve here
  size_t candidate{};
  return answer;
}

std::optional<std::string> day(PuzzleArgs puzzle_args) {
  aoc::print(
    "\nday part:{} debug:{}"
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
