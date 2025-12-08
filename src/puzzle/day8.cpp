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

  // Example or full input?
  int N = (std::string_view(puzzle_args.in_file_path().filename().string()).starts_with("ex"))?10:1000;
  auto test_ix = puzzle_args.meta().m_maybe_test.value_or(0);

  if (test_ix == 5) N = 1;
  else if (test_ix == 6) N = 2;

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

  // Union find by counts
  std::vector<int> parents(model.size(),0);
  for (int i=0;i<model.size();++i) parents[i] = i;

  std::vector<int> counts(model.size(),1);

  // Debug log
  auto print_vec = [](std::string_view caption,std::vector<int> const& vec,bool print_ix=false){
    if (print_ix) {
      aoc::print("\n{:10}:","ix:");
      for (size_t i=0;i<vec.size();++i) aoc::print(" {:>4}",std::to_string(i).back());
    }
    aoc::print("\n{:10}:",caption);
    for (auto val : vec) aoc::print(" {:4}",val);
  };

  auto find_root = [&parents](int x) {
    int current = x;
    while (parents[current] != current) {
      current = parents[current]; // back track
    }
    return current;
  };

  for (int i=0;i<N;++i) {
    aoc::print("\n\n{}:",i);
    auto const& [edge,dps] = edges[i];
    auto first_root = find_root(edge.first);
    auto second_root = find_root(edge.second);

    if (second_root == first_root) continue; // Skip (already same union)

    // aggregate to largets
    if (counts[first_root] > counts[second_root]) {
      std::swap(first_root,second_root);
    }

    aoc::print(
     " connect ({}::{})[u:{} c:{}]"
    ,to_string(model[edge.first])
    ,edge.first
    ,first_root
    ,counts[first_root]);

    aoc::print(
     " <-- ({}::{})[u:{} c:{}]"
    ,to_string(model[edge.second])
    ,edge.second
    ,second_root
    ,counts[second_root]);

    parents[second_root] = parents[first_root]; // connect 'smaller' to 'larger'
    counts[first_root] += counts[second_root]; // carry over connected counts


    print_vec("parents",parents,true);
    print_vec("counts:",counts);
  }

  // Examine how many unions we have (each union have a unique root in parents)
  std::set<int> unique_parents{};
  for (auto parent : parents) unique_parents.insert(find_root(parent));

  std::vector<int> sorted_parents(unique_parents.begin(),unique_parents.end());
  std::ranges::sort(sorted_parents,[&counts](auto lhs,auto rhs){
    return counts[lhs] > counts[rhs];
  });

  switch (test_ix) {
    case 5: // one edge
    case 6: // two edges
    case 7: /* 10 edges */  {
      auto r0 = sorted_parents[0];
      auto r1 = sorted_parents[1];
      auto r2 = sorted_parents[2];
      auto r3 = sorted_parents[3];
      return std::format(
        "{},{},{},{},{}"
        ,unique_parents.size()
        ,counts[r0]
        ,counts[r1]
        ,counts[r2]
        ,counts[r3]);
    } break;
    default: break;
  }

  size_t candidate{1};
  for (int i=0;i<3;++i) {
    auto root = sorted_parents[i];
    auto size = counts[root];
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
