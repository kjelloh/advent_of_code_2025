#include "aoc25.h"
#include <string>
#include <print>
#include <iostream>
#include <ranges>
#include <fstream>
#include <utility> // std::make_pair
#include <numeric> // std::accumulate,...
#include <set>

size_t p1(PuzzleArgs puzzle_args) {
  size_t answer{};
  std::print("\nday03");
  std::ifstream in{puzzle_args.in_file_path()};
  std::string entry;
  while (std::getline(in,entry)) {
    std::print("\n{} ",entry);
    auto picked = std::make_pair('0','0');
    for (int i=0;i<entry.size()-1;++i) {
      if (picked.first < entry[i]) {
        picked.first = entry[i];
        picked.second = entry[i+1];
        for (int j=i+1;j<entry.size();++j) {
          picked.second = std::max(picked.second,entry[j]);
        }
      }
    }
    std::print(" picked {}{}",picked.first,picked.second);
    answer += (picked.first - '0')*10 + picked.second - '0';
  }
  // std::print("\nANSWER: {}",answer);

  return answer;
}

size_t p2(PuzzleArgs puzzle_args) {
  size_t answer{};
  std::print("\nday03");
  std::ifstream in{puzzle_args.in_file_path()};
  std::string entry;
  while (std::getline(in,entry)) {
    std::print("\n{} ",entry);


    // We can solve this by finding the largest monotonic decerasing stack of digits from the input
    auto to_remove = entry.size() - 12;
    std::stack<char> stack{};

    for (size_t i=0;i<entry.size();++i) {

      // Dispose stacked digits 'weaker' than current entry[i] if we can
      while (
            not stack.empty() 
        and to_remove > 0 
        and entry[i] > stack.top()) {

        stack.pop();
        --to_remove;

      }

      stack.push(entry[i]); // push the new one

    }

    while (stack.size() > 12) stack.pop();

    // Empty the stack to a digit string
    std::string picked{};
    while (not stack.empty()) {
      picked += stack.top();
      stack.pop();
    }

    // Reverse it to get the digit in the correct order (oldest = first)
    std::reverse(picked.begin(),picked.end());

    auto optimized = std::stoll(picked);

    std::print(" optimized:{}",optimized);

    answer += optimized;

  }
  // std::print("\nANSWER: {}",answer); // 3121910778619
  //                                    // 3121910778619
  return answer;
}


std::string day(PuzzleArgs puzzle_args) {
  auto answer1 = p1(puzzle_args);
  auto answer2 = p2(puzzle_args);
  return std::format("p1:{} p2:{}",answer1,answer2);
}
