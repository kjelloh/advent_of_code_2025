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

using INT = int64_t;
using UINT = uint64_t;

using AdjacencyList = std::map<std::string,std::set<std::string>>;
using Model = AdjacencyList;
Model parse(std::istream& in) {
  Model model{};

  std::string entry;
  int ix{0};
  while (std::getline(in, entry)) {
    aoc::print("\nin[{:4}][0..{:3}]: '{}' ", ix++,entry.size()-1,entry);
    std::istringstream iss{entry};
    std::string parent;
    std::getline(iss, parent, ':');
    aoc::print(" ==> '{}' -> ",parent);
    std::string child;
    while (iss >> child) {
      model[parent].insert(child);
      aoc::print(" '{}'",child);
    }
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

namespace diagnostics {
  int shortest_path_bfs(
     const Model& graph
    ,const std::string& start
    ,const std::string& end) {

    std::queue<std::pair<std::string, int>> q;
    std::set<std::string> visited;
    q.push({start, 0});
    visited.insert(start);
    
    while (!q.empty()) {
      auto [current, dist] = q.front();
      q.pop();
      
      if (current == end) return dist;
      
      if (graph.contains(current)) {
        for (const auto& neighbor : graph.at(current)) {
          if (!visited.contains(neighbor)) {
              visited.insert(neighbor);
              q.push({neighbor, dist + 1});
          }
        }
      }
    }
    return -1; // Not reachable
  }

  bool has_cycle_from(
     const Model& graph
    ,const std::string& node
    ,std::set<std::string>& visited
    ,std::set<std::string>& rec_stack) {

    visited.insert(node);
    rec_stack.insert(node);
    
    if (graph.contains(node)) {
      for (const auto& neighbor : graph.at(node)) {
        // If neighbor is in recursion stack, we found a cycle!
        if (rec_stack.contains(neighbor)) {
          return true;
        }
        
        // If not visited, recurse
        if (!visited.contains(neighbor)) {
          if (has_cycle_from(graph, neighbor, visited, rec_stack)) {
            return true;
          }
        }
      }
    }
    
    rec_stack.erase(node);  // Remove from recursion stack
    return false;
  }

  bool graph_has_cycles(const Model& graph) {
    std::set<std::string> visited;
    std::set<std::string> rec_stack;
    
    for (const auto& [node, _] : graph) {
      if (!visited.contains(node)) {
        if (has_cycle_from(graph, node, visited, rec_stack)) {
          return true;
        }
      }
    }
    
    return false;
  }  
} // diagnostics

UINT count_to_target_dfs(
   const Model& graph
  ,const std::string& current
  ,const std::string& target
  ,std::map<std::string,size_t>& memo) {

  UINT total = 0;

  static UINT call_count{};

  if (call_count++ % 100000 == 0) {
    aoc::print(
       "\n{}: '{}' -> '{} {} "
      ,call_count
      ,current
      ,target
      ,total);
    std::cout << std::flush;
  }

  if (current == target) {
    return 1;
  }

  if (memo.contains(current)) {
    return memo[current];
  }

  if (graph.contains(current)) {
    for (const auto& neighbor : graph.at(current)) {
        total += count_to_target_dfs(
            graph
          ,neighbor
          ,target
          ,memo);
    }
  }
  memo[current] = total;
  return total;
}

UINT count_to_target_dfs(
   const Model& graph
  ,const std::string& start
  ,const std::string& end) {

  std::map<std::string,size_t> memo{};
  return count_to_target_dfs(graph,start,end,memo);
}


std::optional<std::string> solve(PuzzleArgs puzzle_args,bool for_part2 = false) {

  auto test_ix = puzzle_args.meta().m_maybe_test.value_or(0);

  std::ifstream in{puzzle_args.in_file_path()};
  auto model = parse(in);

  // Solve here
  std::string start("you");
  std::string end("out");
  UINT candidate{};
  
  if (for_part2) {
    start = "svr";

    // We need to know more about the graph to tackle this problem
    auto const& graph = model;
    int node_count = graph.size();
    int edge_count = 0;
    for (const auto& [node, neighbors] : graph) {
        edge_count += neighbors.size();
    }
    aoc::print("\nV:{} E:{}",node_count,edge_count); // V:574 E:1650

    {
      auto sp = diagnostics::shortest_path_bfs(
        graph
        ,start
        ,end);
      aoc::print(
        "\nShortest path '{}' -> '{}' = {}"
        ,start
        ,end
        ,sp);
    }

    {
      auto start = "fft";
      auto end = "dac";
      auto sp = diagnostics::shortest_path_bfs(
        graph
        ,start
        ,end);
      aoc::print(
        "\nShortest path '{}' -> '{}' = {}"
        ,start
        ,end
        ,sp);
    }
    {
      auto start = "dac";
      auto end = "fft";
      auto sp = diagnostics::shortest_path_bfs(
        graph
        ,start
        ,end);
      aoc::print(
        "\nShortest path '{}' -> '{}' = {}"
        ,start
        ,end
        ,sp);
    }
    {
      auto start = "svr";
      auto end = "fft";
      auto sp = diagnostics::shortest_path_bfs(
        graph
        ,start
        ,end);
      aoc::print(
        "\nShortest path '{}' -> '{}' = {}"
        ,start
        ,end
        ,sp);
    }
    {
      auto start = "dac";
      auto sp = diagnostics::shortest_path_bfs(
        graph
        ,start
        ,end);
      aoc::print(
        "\nShortest path '{}' -> '{}' = {}"
        ,start
        ,end
        ,sp);
    }

    {
      auto has_cycles = diagnostics::graph_has_cycles(graph);
      aoc::print(
         "\n{} has cycles:{} "
        ,puzzle_args.in_file_path().filename().string()
        ,has_cycles);
    }

    // No cycles
    auto svr_to_fft_count = count_to_target_dfs(model,"svr","fft");
    auto fft_to_dac_count = count_to_target_dfs(model,"fft","dac");
    auto dac_to_out_count = count_to_target_dfs(model,"dac","out");
    candidate = svr_to_fft_count * fft_to_dac_count * dac_to_out_count;
    aoc::print(
       "\n{} * {} * {} = {}"
      ,svr_to_fft_count
      ,fft_to_dac_count
      ,dac_to_out_count
      ,candidate);
  }
  else {
    candidate = count_to_target_dfs(model,start,end);
  }

  // return {};
  // return std::format("Not yet fully implemented");
  return std::format("{}",candidate);
    // 1080 is too low

} // solve


std::optional<std::string> p1(PuzzleArgs puzzle_args) {
  return solve(puzzle_args,false); 
} // p1

std::optional<std::string> p2(PuzzleArgs puzzle_args) {
  return solve(puzzle_args,true);
} // p2

std::optional<std::string> day(PuzzleArgs puzzle_args) {
  aoc::print(
    "\nday11 part:{} debug:{}"
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
