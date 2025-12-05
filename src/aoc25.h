#pragma once

#include <vector>
#include <string>
#include <filesystem>


#ifdef _WIN32
  #define AOC25_EXPORT __declspec(dllexport)
#else
  #define AOC25_EXPORT
#endif

class PuzzleArgs {

public:
  struct Meta {
    bool m_is_part_1{true};
  };

  PuzzleArgs(Meta meta,std::filesystem::path in_file_path);
  
  Meta const&  meta() const {return m_meta;}
  std::filesystem::path const& in_file_path() const {return m_in_file_path;}

private:

  Meta m_meta{};
  std::filesystem::path m_in_file_path;
};

struct AppArgs {
  int part;
  bool debug;
  std::string input_file_stem;
};

int solve(AppArgs args);

AOC25_EXPORT void print_platform();
