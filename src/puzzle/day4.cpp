#include <string>
#include <print>
#include <iostream>
#include <ranges>
#include <fstream>
#include <utility> // std::make_pair
#include <numeric> // std::accumulate,...
#include <set>
#include <optional>

std::optional<size_t> p1(std::filesystem::path in_file_path) {
  std::optional<size_t> answer{};
  std::ifstream in{in_file_path};
  std::string entry;
  while (std::getline(in,entry)) {
    std::print("\n{} ",entry);
  }

  return answer;
}

std::optional<size_t> p2(std::filesystem::path in_file_path) {
  std::optional<size_t> answer{};
  std::ifstream in{in_file_path};

  std::vector<std::string> grid{};

  std::string entry;
  while (std::getline(in,entry)) {
    std::print("\n{} ",entry);
    grid.push_back(entry);
  }

  size_t acc{};
  for (size_t r=0;r<grid.size();++r) {
    for (size_t c=0;c<grid[0].size();++c) {
      if (grid[r][c] == '@') {
        size_t count{};
        for (auto dp : std::vector<std::pair<int,int>>{
           {-1,0}
          ,{-1,1}
          ,{0,1}
          ,{1,1}
          ,{1,0}
          ,{1,-1}
          ,{0,-1}
          ,{-1,-1}
        }) {

          auto nr = r + dp.first;
          auto nc = c + dp.second;

          if (nr < 0) continue;
          if (nr >= grid.size()) continue;
          if (nc < 0) continue;
          if (nc >= grid[0].size()) continue;
          
          if (grid[nr][nc] == '@') ++count;

        }

        if (count < 4) ++acc;

      }
    }
  }

  answer = acc;

  return answer;
}


std::string day(std::filesystem::path in_file_path) {
  std::print("\nday04");
  std::string result{};
  if (auto answer = p1(in_file_path)) {
    result += std::format("p1:{}",*answer);
  }
  if (auto answer = p2(in_file_path)) {
    result += std::format(" p2:{}",*answer);
  }
  return result;
}
