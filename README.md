# advent_of_code_2025

My solutions to ADvent of Cod 2025

## Build and Run

You can try the z-shell scripts (tested on macOS) to init the tool chain (build environment) and and compile and run the code.

* init_tool_chain.zsh

It basically does two things:

1. Tells conan to 'install' the requirements specified in conanfile.py
2. Tells cmake what 'preset' to use

```sh
conan install . --settings=compiler.cppstd=23 --settings=build_type=$BUILD_TYPE --build=missing
cmake --preset $PRESET_NAME
```

* run.zsh

It basically does three things:

1. Tells cmake to build the executable
2. Copies it to 'workspace'
3. Executes it (passing user arguments)

```sh
cmake --build --preset $PRESET_NAME
cp "build/$BUILD_TYPE/$EXECUTABLE" "$WORKSPACE_DIR/"
./$EXECUTABLE "$@"
```

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
