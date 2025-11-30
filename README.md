# advent_of_code_2025

My solutions to ADvent of Cod 2025

## conan 2.0 support

Scaffholding has been created with 'conan new cmake_exe ...'

```sh
kjell-olovhogdahl@MacBook-Pro ~/Documents/GitHub/advent_of_code_2025 % conan new cmake_exe -d name=aoc25 -d version=1.0
File saved: CMakeLists.txt
File saved: conanfile.py
File saved: src/aoc25.cpp
File saved: src/aoc25.h
File saved: src/main.cpp
File saved: test_package/conanfile.py
kjell-olovhogdahl@MacBook-Pro ~/Documents/GitHub/advent_of_code_2025 % code .
```

To make conan 'the requirements specified in conanfile.py', do 'conan install ...'.

Example:

```sh
kjell-olovhogdahl@MacBook-Pro ~/Documents/GitHub/advent_of_code_2025 % conan install . --output-folder=build --build=missing

======== Input profiles ========
Profile host:
[settings]
arch=armv8
build_type=Release
compiler=apple-clang
compiler.cppstd=gnu17
compiler.libcxx=libc++
compiler.version=16
os=Macos

Profile build:
[settings]
arch=armv8
build_type=Release
compiler=apple-clang
compiler.cppstd=gnu17
compiler.libcxx=libc++
compiler.version=16
os=Macos


======== Computing dependency graph ========
Graph root
    conanfile.py (aoc25/1.0): /Users/kjell-olovhogdahl/Documents/GitHub/advent_of_code_2025/conanfile.py

======== Computing necessary packages ========

======== Installing packages ========

======== Finalizing install (deploy, generators) ========
conanfile.py (aoc25/1.0): Calling generate()
conanfile.py (aoc25/1.0): Generators folder: /Users/kjell-olovhogdahl/Documents/GitHub/advent_of_code_2025/build/build/Release/generators
conanfile.py (aoc25/1.0): CMakeToolchain generated: conan_toolchain.cmake
conanfile.py (aoc25/1.0): CMakeToolchain: Preset 'conan-release' added to CMakePresets.json.
    (cmake>=3.23) cmake --preset conan-release
    (cmake<3.23) cmake <path> -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=generators/conan_toolchain.cmake  -DCMAKE_POLICY_DEFAULT_CMP0091=NEW -DCMAKE_BUILD_TYPE=Release
conanfile.py (aoc25/1.0): CMakeToolchain generated: /Users/kjell-olovhogdahl/Documents/GitHub/advent_of_code_2025/build/build/Release/generators/CMakePresets.json
conanfile.py (aoc25/1.0): CMakeToolchain generated: /Users/kjell-olovhogdahl/Documents/GitHub/advent_of_code_2025/CMakeUserPresets.json
conanfile.py (aoc25/1.0): Generating aggregated env files
conanfile.py (aoc25/1.0): Generated aggregated env files: ['conanbuild.sh', 'conanrun.sh']
Install finished successfully
kjell-olovhogdahl@MacBook-Pro ~/Documents/GitHub/advent_of_code_2025 % 
```

## CMake support

After you told conan to do 'its thing', you tell cmake to take over.

To have cmake create the tool chain of youit platform, create the build directory, cd into it and engage cmake on '..' (where CMakeLists.txt is).

Example:

```sh
kjell-olovhogdahl@MacBook-Pro ~/Documents/GitHub/advent_of_code_2025 % mkdir build 
kjell-olovhogdahl@MacBook-Pro ~/Documents/GitHub/advent_of_code_2025 % cd build
kjell-olovhogdahl@MacBook-Pro ~/Documents/GitHub/advent_of_code_2025/build % cmake ..
-- The CXX compiler identification is AppleClang 16.0.0.16000026
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done (0.2s)
-- Generating done (0.0s)
-- Build files have been written to: /Users/kjell-olovhogdahl/Documents/GitHub/advent_of_code_2025/build
kjell-olovhogdahl@MacBook-Pro ~/Documents/GitHub/advent_of_code_2025/build % 
```



