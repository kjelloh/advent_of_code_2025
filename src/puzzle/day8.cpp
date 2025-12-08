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

using Integer= long long;

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

Integer squared_distance(Point lhs,Point rhs) {
  Integer dx = (rhs.x - lhs.x);
  Integer dy = (rhs.y - lhs.y);
  Integer dz = (rhs.z - lhs.z);

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
    std::pair<Edge,Integer>> edges{};
  for (int i=0;i<model.size();++i) {
    for (int j=i+1;j<model.size();++j) {
      auto pi = model[i];
      auto pj = model[j];
      auto dps = squared_distance(pi,pj);
      aoc::print("\ndps:{}",dps);

      if (dps < 0) {
        return std::format("\nFAILED - Overflow to negative squared distance {}",dps);
      }

      Edge edge{i,j};
      edges.push_back(std::make_pair(edge,dps)); // silemtly narrows long long to pair.second type!
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
    if (counts[first_root] < counts[second_root]) {
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


    // print_vec("parents",parents,true);
    // print_vec("counts:",counts);
  }

  // Examine how many unions we have (each union have a unique root in parents)
  std::set<int> roots{};
  for (size_t node=0;node<parents.size();++node) roots.insert(find_root(node));

  std::vector<int> sorted_roots(roots.begin(),roots.end());
  std::ranges::sort(sorted_roots,[&counts](auto lhs,auto rhs){
    return counts[lhs] > counts[rhs];
  });

  print_vec("counts",counts);

  switch (test_ix) {
    case 5: // one edge
    case 6: // two edges
    case 7: /* 10 edges */  {
      auto r0 = sorted_roots[0];
      auto r1 = sorted_roots[1];
      auto r2 = sorted_roots[2];
      auto r3 = sorted_roots[3];
      return std::format(
        "{},{},{},{},{}"
        ,roots.size()
        ,counts[r0]
        ,counts[r1]
        ,counts[r2]
        ,counts[r3]);
    } break;
    default: break;
  }

  size_t candidate{1};
  for (int i=0;i<3;++i) {
    auto root = sorted_roots[i];
    auto size = counts[root];
    candidate *= size;
    aoc::print("\nroot:{} size:{} -> candidate:{}",root,size,candidate);
  }

  answer = std::format("{}",candidate);

  return answer; 
  
  // root:861 size:814 -> candidate:814
  // root:552 size:4 -> candidate:3256
  // root:442 size:4 -> candidate:13024  
  // Too low: 13024

  // root:197 size:62 -> candidate:62
  // root:976 size:46 -> candidate:2852
  // root:691 size:43 -> candidate:122636  
  // Correct: 122636

}

std::optional<std::string> p2(PuzzleArgs puzzle_args) {

  // Example or full input?
  int N = (std::string_view(puzzle_args.in_file_path().filename().string()).starts_with("ex"))?10:1000;
  auto test_ix = puzzle_args.meta().m_maybe_test.value_or(0);

  if (test_ix == 5) N = 1;
  else if (test_ix == 6) N = 2;

  std::optional<std::string> answer{};
  std::ifstream in{puzzle_args.in_file_path()};
  auto model = parse(in);
  // Solve here
  const auto V = model.size();

  // Edges weighted with 'distance'
  const size_t E = V*(V-1)/2;
  std::vector<std::pair<Edge,size_t>> we(E);
  size_t ix{0}; // generate pair ix (combinatorial expression is no joke...)
  for (size_t i=0;i<V;++i) {
    for (size_t j=i+1;j<V;++j) {
      if (ix>= we.size()) return std::format("Can't insert element {} into we of size {}",i*j,we.size());
      we[ix] = std::make_pair(Edge{i,j},squared_distance(model[i],model[j]));
      ++ix;
    }
  }

  std::ranges::sort(we,[](auto const& lhs,auto const& rhs){
    return (lhs.second < rhs.second);
  });

  for (auto const& [edge,dps] : we) {
    aoc::print(
       "\ndps:{} {} - {}"
      ,dps 
      ,to_string(model[edge.first])
      ,to_string(model[edge.second]));
  }

  // Test?
  switch (test_ix) {
    case 1:
    case 2:
    case 3:
    case 4: {
      auto [edge,dps] = we[test_ix-1];
      auto p1 = model[edge.first];
      auto p2 = model[edge.second];
      return std::format(
        "{} and {}"
        ,to_string(p1)
        ,to_string(p2));
    } break;
    default: break;
  }

  // Union find on size (counts)
  std::vector<unsigned> parents(V,0);
  for (unsigned i=0;i<V;++i) parents[i] = i; // self parents = roots
  std::vector<unsigned> counts(V,1); // all unions 1 element

  auto find_root = [&parents](int x) {
    int current = x;
    while (parents[current] != current) {
      current = parents[current]; // back track
    }
    return current;
  };

  unsigned component_count = V; // Start with V single member unions
  Edge last_edge{};
  for (unsigned e=0;e<E;++e) {
    auto const& [edge,dps] = we[e];
    auto const& [v1,v2] = edge;
    auto r1 = find_root(v1);
    auto r2 = find_root(v2);
    if (r1 == r2) continue; // already same union (same root)
    if (counts[r1] < counts[r2]) std::swap(r1,r2); // make '1' the larger one
    // Make r1 the root of tree with root r2
    parents[r2] = r1; // same root (makes all childs of r2 also find root r1 == joined)
    counts[r1] += counts[r2]; // joint size
    --component_count;
    aoc::print(
       "\nAfter {} and {} -> component_count:{}"
      ,to_string(model[v1])
      ,to_string(model[v2])
      ,component_count);
    if (component_count==1) {
      last_edge = edge;
      break;
    }
  }

  auto p1 = model[last_edge.first];
  auto p2 = model[last_edge.second];
  aoc::print(
     "\nLast edge {} and {} "
    ,to_string(p1)
    ,to_string(p2));
  auto candidate = static_cast<uint64_t>(p1.x) * static_cast<uint64_t>(p2.x); // To small for 9271575747?
  return std::format("{}",candidate);

  //  681641155 too low
  // 9271575747

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
