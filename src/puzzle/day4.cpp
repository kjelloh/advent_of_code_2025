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
  std::string entry;
  while (std::getline(in,entry)) {
    std::print("\n{} ",entry);
  }

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
