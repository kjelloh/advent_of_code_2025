#include "aoc25.h"
#include <CLI/CLI.hpp>
#include <vector>
#include <string>
#include <iostream>

int main(int argc, char** argv) {
  CLI::App app{"Advent of Code 2025 Solver"};

  AppArgs args;

  // Define command-line options

  app.add_option("-p,--part", args.part, "Part to solve (0=both, 1=part1, 2=part2)")
     ->check(CLI::Range(0, 2))
     ->default_val(0);

  app.add_option("--in", args.input_file_stem, "Input file stem ex0,ex1,in... (if not provided, scans for .txt files)")
     ->check(CLI::ExistingFile);

  // Add platform info flag
  bool show_platform{false};
  app.add_flag("--platform", show_platform, "Show platform information");

  // Parse arguments
  CLI11_PARSE(app, argc, argv);

  if (show_platform) {
    print_platform();
    return 0;
  }

  return solve(args);
}
