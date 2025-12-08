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
  return std::format("{},{},{}",p.x,p.y,p.z);  
}

auto squared_distance(Point lhs,Point rhs) {
  auto dx = (rhs.x - lhs.x);
  auto dy = (rhs.y - lhs.y);
  auto dz = (rhs.z - lhs.z);

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

std::optional<std::string> p1(PuzzleArgs puzzle_args) {
  std::optional<std::string> answer{};
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
    return (lhs.second < rhs.second);
  });

  for (auto const& entry : edges) {
    aoc::print(
       "\nsorted:{} ({} - {})"
      ,entry.second
      ,to_string(model[entry.first.first])
      ,to_string(model[entry.first.second]));
  }

  // Test?
  auto test_ix = puzzle_args.meta().m_maybe_test.value_or(0);
  switch (test_ix) {
    case 1:
    case 2:
    case 3:
    case 4: {
      auto [edge,dps] = edges[test_ix-1];
      auto p1 = model[edge.first];
      auto p2 = model[edge.second];
      return std::format(
        "{} and {}"
        ,to_string(p1)
        ,to_string(p2));
    } break;
    default: break;
  }
  if (puzzle_args.meta().m_maybe_test.value_or(0) == 1) {
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

  auto print_vec = [](std::string_view caption,std::vector<int> const& vec){
    aoc::print("\n{:10}:",caption);
    for (auto val : vec) aoc::print(" {}",val);
  };

  int N = (std::string_view(puzzle_args.in_file_path().filename().string()).starts_with("ex"))?10:1000;
  int connected_count = 0;
  for (int i=0;i<edges.size();++i) {
    auto const& [edge,dps] = edges[i];
    auto& lhs_root = roots[edge.first];
    auto& rhs_root = roots[edge.second];
    if (rhs_root == lhs_root) continue; // Skip
    rhs_root = lhs_root; // mutate in place (refs)
    ++ranks[lhs_root];
    ++connected_count;

    print_vec("roots",roots);
    print_vec("ranks:",ranks);

    if (connected_count < N) continue;
    break;
  }

  // Examine how many unions we have (each union have a unique root in roots)
  std::set<int> unique_roots{};
  for (auto root : roots) unique_roots.insert(root);

  std::vector<int> sorted_roots(unique_roots.begin(),unique_roots.end());
  std::ranges::sort(sorted_roots,[&ranks](auto lhs,auto rhs){
    return ranks[lhs] > ranks[rhs];
  });

  if (test_ix == 5) {
    auto r0 = sorted_roots[0];
    auto r1 = sorted_roots[1];
    auto r2 = sorted_roots[2];
    auto r3 = sorted_roots[3];
    if (ranks[r2] == ranks[r3]) {
      // "there are 11 circuits: one circuit which contains 5 junction boxes, one circuit which contains 4 junction boxes, two circuits which contain 2 junction boxes"
      return std::format(
        "there are {} circuits: one circuit which contains {} junction boxes, one circuit which contains {} junction boxes, two circuits which contain {} junction boxes"
        ,unique_roots.size()
        ,ranks[r0]
        ,ranks[r1]
        ,ranks[r2]);
    }
    else {
      return std::format("ranks[{}] != ranks[{}]",r2,r3);
    }
  }
  size_t candidate{1};
  for (int i=0;i<3;++i) {
    auto root = sorted_roots[i];
    auto size = ranks[root];
    candidate *= size;
    aoc::print("\nroot:{} size:{} -> candidate:{}",root,size,candidate);
  }

  return answer;
}

std::optional<std::string> p2(PuzzleArgs puzzle_args) {
  std::optional<std::string> answer{};
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
      return p1(puzzle_args);
    } break;
    case 2:
      return p2(puzzle_args);
      break;
  }
  return std::nullopt;
}
