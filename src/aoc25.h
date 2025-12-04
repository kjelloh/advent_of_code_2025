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
  };

  PuzzleArgs(Meta meta,std::filesystem::path in_file_path);
  
  Meta const&  meta() const {return m_meta;}
  std::filesystem::path const& in_file_path() const {return m_in_file_path;}

private:

  Meta m_meta{};
  std::filesystem::path m_in_file_path;
}
;
int solve();

AOC25_EXPORT void print_platform();
