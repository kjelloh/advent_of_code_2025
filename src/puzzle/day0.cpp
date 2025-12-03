#include <filesystem>
#include <fstream>
#include <string>
#include <print>


std::string day(std::filesystem::path in_file_path) {
  int result{};

  std::ifstream in{in_file_path};


  std::string line{};
  while (std::getline(in,line)) {
    std::print("\nin:{}",line);
  }

  return std::format("\nNo operation");

}