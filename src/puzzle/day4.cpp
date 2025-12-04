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

std::optional<size_t> p2(std::filesystem::path in_file_path) {
  std::optional<size_t> answer{};
  std::ifstream in{in_file_path};

  struct V {
    size_t r; 
    size_t c;
    auto operator<=>(V const&) const = default;
  };
  std::set<V> grid{};

  size_t r=0;
  size_t width{};
  std::string entry;
  while (std::getline(in,entry)) {
    std::print("\n{} ",entry);
    width = std::max(width,entry.size());
    for (size_t c=0;c<entry.size();++c) {
      if (entry[c] == '@') grid.insert(V{r,c});
    }
    ++r;
  }
  size_t height{r};

  size_t acc{};
  while (true) {
    std::set<V> to_remove{};
    for (auto v : grid) {
      if (grid.contains(v)) {
        size_t count{};
        for (auto dv : std::vector<std::pair<int,int>>{
          {-1,0}
          ,{-1,1}
          ,{0,1}
          ,{1,1}
          ,{1,0}
          ,{1,-1}
          ,{0,-1}
          ,{-1,-1}
        }) {

          V nv{v.r+dv.first,v.c+dv.second};
          auto [nr,nc] = nv;

          if (nr < 0) continue;
          if (nr >= height) continue;
          if (nc < 0) continue;
          if (nc >= width) continue;
          
          if (grid.contains(nv)) ++count;

        }

        if (count < 4) to_remove.insert(v);

      }
    } // for v in grid
    std::print("\nRemove {} rolls of paper",to_remove.size());
    acc += to_remove.size();
    if (to_remove.size() == 0) break;

    std::set<V> next{};
    for (auto v : grid) {
      if (not to_remove.contains(v)) next.insert(v);
    }
    grid = next;
  } // while

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
