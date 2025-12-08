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

struct Lights {
  Point p1;
  Point p2;
  auto squared_length() const {
    return squared_distance(p1,p2);
  }
};

using Node = int;
using Edge = std::pair<Node,Node>;

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
  std::vector<
    std::pair<Edge,int>> edges{};
  for (int i=0;i<model.size();++i) {
    for (int j=i+1;j<model.size();++j) {
      auto pi = model[i];
      auto pj = model[j];
      auto dps = squared_distance(pi,pj);
      aoc::print("\ndps:{}",dps);
      Edge edge{i,j};
      edges.push_back(std::make_pair(edge,dps));
    }
  }
  std::ranges::sort(edges,[](auto const& lhs,auto const& rhs){
    return (lhs.second > rhs.second);
  });

  for (auto const& entry : edges) {
    aoc::print(
       "\nsorted:{} ({} - {})"
      ,entry.second
      ,to_string(model[entry.first.first])
      ,to_string(model[entry.first.second]));
  }
  
  std::map<Node,std::set<Node>> adjacent{};
  for (auto const& entry : edges) {
    auto const& [edge,squared_distance] = entry;
    auto const& [n1,n2] = edge;
    adjacent[n1].insert(n2);
    adjacent[n2].insert(n1);
  }

  // Split into disjoint subgraphs
  std::vector<int> roots(model.size(),0);
  for (int i=0;i<model.size();++i) roots[i] = i;
  std::vector<int> ranks(model.size(),1);

  for (auto const& edge : edges) {
    auto& lhs_root = roots[edge.first.first];
    auto& rhs_root = roots[edge.first.second];
    if (rhs_root == lhs_root) continue; // Already jointed
    rhs_root = lhs_root; // mutate in place (refs)
  }

  // Examine how many unions we have (each union have a unique root in roots)
  std::set<int> unique_roots{};
  for (auto root : roots) unique_roots.insert(root);

  std::vector<int> sorted_roots(unique_roots.begin(),unique_roots.end());
  std::ranges::sort(sorted_roots);

  size_t candidate{1};
  for (int i=0;i<3;++i) {
    auto root = sorted_roots[i];
    auto size = ranks[root];
    candidate *= size;
    aoc::print("\nroot:{} size:{} -> candidate:{}",root,size,candidate);
  }

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
