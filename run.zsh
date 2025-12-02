#!/bin/zsh

EXECUTABLE=aoc25
ERROR_LIMIT=3
SEQUENTIAL_BUILD=true

# Read current preset from file, default to conan-debug if file doesn't exist
if [[ -f "cmake_preset.txt" ]]; then
    PRESET_NAME=$(cat cmake_preset.txt)
    echo "run.zsh: Using preset from cmake_preset.txt: $PRESET_NAME"
    
    # Extract build type from preset name for workspace directory
    case $PRESET_NAME in
        "conan-debug") BUILD_TYPE="Debug" ;;
        "conan-release") BUILD_TYPE="Release" ;;
        "conan-relwithdebinfo") BUILD_TYPE="RelWithDebInfo" ;;
        "conan-minsizerel") BUILD_TYPE="MinSizeRel" ;;
        *) 
            echo "run.zsh: Error: Unknown preset in cmake_preset.txt: $PRESET_NAME"
            echo "run.zsh: Run './init_tool_chain.zsh [Debug|Release|RelWithDebInfo|MinSizeRel]' to fix"
            exit 1
            ;;
    esac
else
    PRESET_NAME="conan-debug"
    BUILD_TYPE="Debug"
    echo "run.zsh: Warning: cmake_preset.txt not found, defaulting to conan-debug"
    echo "run.zsh: Run './init_tool_chain.zsh [Debug|Release|RelWithDebInfo|MinSizeRel]' first to set preset"
fi

# Define the workspace directory
WORKSPACE_DIR="workspace"

# Determine day number
DAY_NUM=""
WORKSPACE_EXISTED=false

# Check if workspace exists before we create it
if [[ -d "$WORKSPACE_DIR" ]]; then
    WORKSPACE_EXISTED=true
fi

# First, try to get day number from first argument
DAY_FROM_ARG=false
if [[ -n "$1" ]] && [[ "$1" =~ ^[0-9]+$ ]] && [[ "$1" -ge 0 ]] && [[ "$1" -le 25 ]]; then
    DAY_NUM=$(printf "%d" "$1")
    DAY_FROM_ARG=true
    echo "run.zsh: Using day number from argument: $DAY_NUM"
    # Remove the day argument from the argument list
    shift
# If workspace exists, try to read from day.txt
elif [[ "$WORKSPACE_EXISTED" == "true" ]] && [[ -f "$WORKSPACE_DIR/day.txt" ]]; then
    DAY_NUM_RAW=$(cat "$WORKSPACE_DIR/day.txt" 2>/dev/null | tr -d '[:space:]')
    if [[ -n "$DAY_NUM_RAW" ]] && [[ "$DAY_NUM_RAW" =~ ^[0-9]+$ ]] && [[ "$DAY_NUM_RAW" -ge 0 ]] && [[ "$DAY_NUM_RAW" -le 25 ]]; then
        DAY_NUM=$(printf "%02d" "$DAY_NUM_RAW")
        echo "run.zsh: Using day number from $WORKSPACE_DIR/day.txt: $DAY_NUM"
    else
        DAY_NUM="00"
        echo "run.zsh: Invalid day number in $WORKSPACE_DIR/day.txt, using fallback: $DAY_NUM"
    fi
else
    DAY_NUM="00"
    echo "run.zsh: No valid day number found, using fallback: $DAY_NUM"
fi

# Define the day-specific workspace subdirectory
DAY_WORKSPACE_DIR="$WORKSPACE_DIR/day$DAY_NUM"

echo "run.zsh: Running $BUILD_TYPE build with preset $PRESET_NAME..."

# Create the workspace directory if it doesn't exist
echo "run.zsh: Creating workspace directory..."
mkdir -p "$DAY_WORKSPACE_DIR"

# Create or update day.txt with the day number
if [[ "$WORKSPACE_EXISTED" == "false" ]]; then
    echo "$DAY_NUM" > "$WORKSPACE_DIR/day.txt"
    echo "run.zsh: Created $WORKSPACE_DIR/day.txt with day number: $DAY_NUM"
elif [[ "$DAY_FROM_ARG" == "true" ]]; then
    echo "$DAY_NUM" > "$WORKSPACE_DIR/day.txt"
    echo "run.zsh: Updated $WORKSPACE_DIR/day.txt with day number: $DAY_NUM"
fi

# Configure with error limit, then build
echo "run.zsh: Configuring with error limit..."
cmake --preset $PRESET_NAME -DMAX_COMPILE_ERRORS=$ERROR_LIMIT

echo "run.zsh: Building the project with cmake..."
if [[ "$SEQUENTIAL_BUILD" == "true" ]]; then
    echo "run.zsh: Using sequential build (SEQUENTIAL_BUILD=$SEQUENTIAL_BUILD)"
    cmake --build --preset $PRESET_NAME --parallel 1
else
    echo "run.zsh: Using parallel build (SEQUENTIAL_BUILD=$SEQUENTIAL_BUILD)"
    cmake --build --preset $PRESET_NAME
fi

# Check if the build was successful
if [[ $? -ne 0 ]]; then
    echo "run.zsh: Build failed. Exiting."
    exit 1
fi

# Copy the built binary to the day-specific workspace directory
echo "run.zsh: Copying the '$EXECUTABLE' binary to $DAY_WORKSPACE_DIR..."
cp "build/$BUILD_TYPE/$EXECUTABLE" "$DAY_WORKSPACE_DIR/"

# Change to the day-specific workspace directory
cd "$DAY_WORKSPACE_DIR"

./$EXECUTABLE "$@"
