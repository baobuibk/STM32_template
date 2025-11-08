# -----------------------------------------------------------------------------
# Toolchain file for building on host (native build on Windows)
# -----------------------------------------------------------------------------
# @author: Generated for Windows host platform
# -----------------------------------------------------------------------------
# @attention
#
# This is a simple toolchain file for native builds on Windows using MinGW (GCC-based).
# Assumes MinGW-w64 is installed and in PATH (e.g., via MSYS2 or standalone).
# For MSVC (Visual Studio), it's better to use CMake defaults without this file,
# as CMake auto-detects it. Set CMAKE_GENERATOR to "Visual Studio 17 2022" or similar.
#
# To use: cmake -DCMAKE_TOOLCHAIN_FILE=toolchainfile.cmake ..
#
# -----------------------------------------------------------------------------

# Set the system name and processor to match the host
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)  # Adjust to i686 for 32-bit if needed

# Use MinGW compilers (CMake will auto-detect if in PATH, but specify explicitly)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_RC_COMPILER windres)  # For resource compiler if needed

# Default flags for host build (optimize for performance, enable warnings)
# MinGW uses GCC flags
set(CMAKE_C_FLAGS "-Wall -Wextra -O2 -march=native")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++17")  # Adjust C++ standard as needed

# Executable suffix for Windows
set(CMAKE_EXECUTABLE_SUFFIX ".exe")

# No need for objcopy/size as in embedded toolchains
# Use default linkers (MinGW ld)

# Ensure executable for try-compile
set(CMAKE_TRY_COMPILE_TARGET_TYPE EXECUTABLE)