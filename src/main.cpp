#include "aoc25.h"
#include <CLI/CLI.hpp>
#include <vector>
#include <string>
#include <iostream>

int main(int argc, char** argv) {
  CLI::App app{"Advent of Code 2025 Solver"};

  AppArgs args;

  // Define command-line options

  app.add_flag("--take2", args.use_take2, "Show platform information");

  app.add_option("-D,--Day", args.day, "Part to solve (1=part1, 2=part2)")
     ->default_val(0)
     ->check(CLI::Range(0, 12));

  app.add_option("-p,--part", args.maybe_part, "Part to solve (1=part1, 2=part2)")
     ->check(CLI::Range(1, 2));

  app.add_flag("-d,--debug", args.debug, "Enable DEBUG output logging");

  app.add_option("--in", args.input_file_name, "Input file ex0.txt,in.txt... (if not provided, scans for .txt files)")
     ->check(CLI::ExistingFile);

  app.add_option("-t,--test", args.maybe_test, "Inject a test number to solver");

  app.add_option("-a,--answer",args.maybe_answer,"Known answer as formatted by solver (if provided solver will compare)");

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
