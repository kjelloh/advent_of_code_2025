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
    int m_part{1};
    bool m_debug{false};
    std::optional<int> m_maybe_test;
  };

  PuzzleArgs(Meta meta,std::filesystem::path in_file_path);
  
  Meta const&  meta() const {return m_meta;}
  std::filesystem::path const& in_file_path() const {return m_in_file_path;}

private:

  Meta m_meta{};
  std::filesystem::path m_in_file_path;
};

struct AppArgs {
  std::optional<int> maybe_test;
  std::optional<int> maybe_part;
  bool debug;
  std::string input_file_name;
  std::optional<std::string> maybe_answer;
};

namespace aoc {
  inline bool print_is_enabled{false};
  template <typename... Args>
  void print(std::format_string<Args...> fmt, Args&&... args) {
    if (print_is_enabled) std::print(fmt, std::forward<Args>(args)...);
  }
}

int solve(AppArgs args);

AOC25_EXPORT void print_platform();
