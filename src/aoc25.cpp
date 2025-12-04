#include "aoc25.h"
#include <iostream>
#include <filesystem> // std::filesystem::path,...

PuzzleArgs::PuzzleArgs(Meta meta,std::filesystem::path in_file_path)
  : m_meta{meta},m_in_file_path{in_file_path} {}

std::string day(PuzzleArgs puzzle_args);

int solve() {

  std::vector<std::string> answers{};

  for (const auto& entry : std::filesystem::directory_iterator(".")) {
    if (entry.is_regular_file() && entry.path().extension() == ".txt") {
        std::cout << entry.path() << std::endl;
        PuzzleArgs puzzle_args({},entry.path());
        answers.push_back(day(puzzle_args));
    }
  }

  int i{};
  for (auto const& answer : answers) {
    std::print("\nANSWER {:2} : {}",i++,answer);
  }
  return 0;
}

void print_platform(){
    

    #ifdef NDEBUG
    std::cout << "aoc25/1.0: Hello World Release!\n";
    #else
    std::cout << "aoc25/1.0: Hello World Debug!\n";
    #endif

    // ARCHITECTURES
    #ifdef _M_X64
    std::cout << "  aoc25/1.0: _M_X64 defined\n";
    #endif

    #ifdef _M_IX86
    std::cout << "  aoc25/1.0: _M_IX86 defined\n";
    #endif

    #ifdef _M_ARM64
    std::cout << "  aoc25/1.0: _M_ARM64 defined\n";
    #endif

    #if __i386__
    std::cout << "  aoc25/1.0: __i386__ defined\n";
    #endif

    #if __x86_64__
    std::cout << "  aoc25/1.0: __x86_64__ defined\n";
    #endif

    #if __aarch64__
    std::cout << "  aoc25/1.0: __aarch64__ defined\n";
    #endif

    // Libstdc++
    #if defined _GLIBCXX_USE_CXX11_ABI
    std::cout << "  aoc25/1.0: _GLIBCXX_USE_CXX11_ABI "<< _GLIBCXX_USE_CXX11_ABI << "\n";
    #endif

    // MSVC runtime
    #if defined(_DEBUG)
        #if defined(_MT) && defined(_DLL)
        std::cout << "  aoc25/1.0: MSVC runtime: MultiThreadedDebugDLL\n";
        #elif defined(_MT)
        std::cout << "  aoc25/1.0: MSVC runtime: MultiThreadedDebug\n";
        #endif
    #else
        #if defined(_MT) && defined(_DLL)
        std::cout << "  aoc25/1.0: MSVC runtime: MultiThreadedDLL\n";
        #elif defined(_MT)
        std::cout << "  aoc25/1.0: MSVC runtime: MultiThreaded\n";
        #endif
    #endif

    // COMPILER VERSIONS
    #if _MSC_VER
    std::cout << "  aoc25/1.0: _MSC_VER" << _MSC_VER<< "\n";
    #endif

    #if _MSVC_LANG
    std::cout << "  aoc25/1.0: _MSVC_LANG" << _MSVC_LANG<< "\n";
    #endif

    #if __cplusplus
    std::cout << "  aoc25/1.0: __cplusplus" << __cplusplus<< "\n";
    #endif

    #if __INTEL_COMPILER
    std::cout << "  aoc25/1.0: __INTEL_COMPILER" << __INTEL_COMPILER<< "\n";
    #endif

    #if __GNUC__
    std::cout << "  aoc25/1.0: __GNUC__" << __GNUC__<< "\n";
    #endif

    #if __GNUC_MINOR__
    std::cout << "  aoc25/1.0: __GNUC_MINOR__" << __GNUC_MINOR__<< "\n";
    #endif

    #if __clang_major__
    std::cout << "  aoc25/1.0: __clang_major__" << __clang_major__<< "\n";
    #endif

    #if __clang_minor__
    std::cout << "  aoc25/1.0: __clang_minor__" << __clang_minor__<< "\n";
    #endif

    #if __apple_build_version__
    std::cout << "  aoc25/1.0: __apple_build_version__" << __apple_build_version__<< "\n";
    #endif

    // SUBSYSTEMS

    #if __MSYS__
    std::cout << "  aoc25/1.0: __MSYS__" << __MSYS__<< "\n";
    #endif

    #if __MINGW32__
    std::cout << "  aoc25/1.0: __MINGW32__" << __MINGW32__<< "\n";
    #endif

    #if __MINGW64__
    std::cout << "  aoc25/1.0: __MINGW64__" << __MINGW64__<< "\n";
    #endif

    #if __CYGWIN__
    std::cout << "  aoc25/1.0: __CYGWIN__" << __CYGWIN__<< "\n";
    #endif
}
