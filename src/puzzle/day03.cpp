#include <string>
#include <print>
#include <iostream>
#include <ranges>
#include <fstream>
#include <utility> // std::make_pair
#include <numeric> // std::accumulate,...
#include <set>

int day03() {
  size_t answer{};
  std::print("\nday03");
  // std::ifstream in{"in.txt"};
  std::ifstream in{"ex0.txt"};
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
  std::print("\nANSWER: {}",answer);


  return 0;
}