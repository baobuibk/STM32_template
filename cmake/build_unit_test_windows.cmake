################################################################################################
# file:  CMakeLists.txt  (Windows host build)
# brief: Template cho build executable/static library trên Windows (MSVC/MinGW/Clang).
#
# usage:
#   # MSVC (Developer Prompt hoặc VS Code CMake Tools)
#   cmake -S . -B build-win -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Debug
#   cmake --build build-win --config Debug
#
#   # MinGW (GCC) hoặc Clang trên Windows:
#   cmake -S . -B build-win -G "Ninja" -DCMAKE_BUILD_TYPE=Debug
#   cmake --build build-win -j
################################################################################################
cmake_minimum_required(VERSION 3.20)

###################### CONSTANTS ######################################
set(PROJECT_TYPE_EXECUTABLE     "exe")
set(PROJECT_TYPE_STATIC_LIBRARY "static-lib")
#######################################################################

###################### VARIABLES (EDIT ME) ############################
# Kiểu project: exe hoặc static-lib
set(PROJECT_TYPE "exe")



# C chuẩn
set(CMAKE_C_STANDARD 11)

# Danh sách source (host)
set(PROJECT_SOURCES
    # LIST SOURCE FILES HERE (host/native)
    # ví dụ: src/main_host.c
    # ${CMAKE_CURRENT_SOURCE_DIR}/test/host_test/test_runner.c
)

# Định nghĩa macro (nếu cần)
set(PROJECT_DEFINES
    # LIST COMPILER DEFINITIONS HERE
    _CRT_SECURE_NO_WARNINGS
    WIN32_LEAN_AND_MEAN
)

# Include directories
set(PROJECT_INCLUDES
    # LIST INCLUDE DIRECTORIES HERE
    # ví dụ: ${CMAKE_SOURCE_DIR}/src
)
#######################################################################

############## PROJECT SETUP (KHÔNG DÙNG CỜ ARM Ở ĐÂY) ################
enable_language(C)

if (${PROJECT_TYPE} MATCHES ${PROJECT_TYPE_EXECUTABLE})
    add_executable(${PROJECT_UNIT_TEST_WINDOWS})

    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
    add_custom_command(TARGET ${PROJECT_UNIT_TEST_WINDOWS} POST_BUILD COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${PROJECT_UNIT_TEST_WINDOWS}>)

else()
    message(FATAL_ERROR "Unknown PROJECT_TYPE: ${PROJECT_TYPE}")
endif()



# ==== Compiler flags cho Windows host ====
if (MSVC)
    # MSVC flags
    target_compile_options(${PROJECT_UNIT_TEST_WINDOWS} PRIVATE
        /W4           # cảnh báo nhiều
        /WX-          # không nâng warning thành lỗi (đặt /WX nếu muốn fail-on-warning)
        /permissive-  # chuẩn hơn
        /Zc:__cplusplus
            # verbose output nếu bạn có define này
    )
    # Link options (Windows)
    target_link_options(${PROJECT_UNIT_TEST_WINDOWS} PRIVATE
        /INCREMENTAL:NO
    )
else()
    # GCC/Clang on Windows (MinGW / Clang + lld)
    target_compile_options(${PROJECT_UNIT_TEST_WINDOWS} PRIVATE
        -Wall -Wextra -Wpedantic
        -Wno-unused-parameter
        -O0 -g
            # verbose output nếu bạn có define này
    )
    # Link options (có thể bỏ nếu không cần)
    target_link_options(${PROJECT_UNIT_TEST_WINDOWS} PRIVATE
        # ví dụ thêm lld: -fuse-ld=lld
    )
endif()
target_compile_definitions(${PROJECT_UNIT_TEST_WINDOWS} PRIVATE
    UNITY_OUTPUT_COLOR
    UNITY_INCLUDE_PRINT_FORMATTED
    UNITY_VERBOSE
)
# ==== Link với Windows system libs nếu cần (ví dụ OSAL Windows port cần winmm) ====
if (WIN32)
    # Nếu bạn dùng osal.c (Windows port) có timeBeginPeriod/timeSetEvent:
    # target_link_libraries(${PROJECT_NAME} PRIVATE winmm)
endif()

