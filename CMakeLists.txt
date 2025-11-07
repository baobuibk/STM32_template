cmake_minimum_required(VERSION 3.20)

project(MySTM32App LANGUAGES C ASM)

# ===========================
# Build options (2 đường build tách bạch)
# ===========================
option(BUILD_FIRMWARE     "Build STM32 firmware (ARM)" ON)
option(BUILD_HOST_TESTS   "Build host-side unit tests (native)" OFF)

# Tên target firmware (do ide_gen tạo ra)
set(FW_TARGET firmware CACHE STRING "Firmware target name")

# ===========================
# Firmware (ARM) - chỉ thêm khi BUILD_FIRMWARE=ON
# ===========================
if(BUILD_FIRMWARE)
  # Tạo target + cấu hình toolchain/linker ở ide_gen
  add_subdirectory(ide_gen)

  # Bơm mã nguồn ứng dụng vào target đã tạo (cung cấp M0_SYSAPP_SOURCES/HEADERS)
  add_subdirectory(src/M0_SysApp)
endif()

# ===========================
# Clang-format (chỉ chạy khi có nguồn M0_SysApp)
# ===========================
if(BUILD_FIRMWARE)
  # Đường dẫn đến file .clang-format
  set(CLANG_FORMAT_FILE "${CMAKE_SOURCE_DIR}/tools/.clang-format")

  # Gom file để format (chỉ khi các biến đã được định nghĩa từ src/M0_SysApp)
  if(DEFINED M0_SYSAPP_SOURCES OR DEFINED M0_SYSAPP_HEADERS)
    set(FORMAT_SOURCES
        ${M0_SYSAPP_SOURCES}
        ${M0_SYSAPP_HEADERS}
    )

    if(NOT FORMAT_SOURCES)
      message(WARNING "No files found in SOURCES or HEADERS for formatting")
    else()
      # Tìm clang-format
      find_program(CLANG_FORMAT NAMES clang-format REQUIRED)

      # Tạo script để chạy lần lượt (ổn định cả Windows & Linux)
      set(FORMAT_SCRIPT "${CMAKE_BINARY_DIR}/cmake/format_m0_sysapp.cmake")
      file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/cmake")

      file(WRITE "${FORMAT_SCRIPT}" "message(STATUS \"Formatting M0_SysApp using ${CLANG_FORMAT_FILE}\")\n")
      foreach(_f IN LISTS FORMAT_SOURCES)
        # In ra tên file đang format
        file(APPEND "${FORMAT_SCRIPT}" "message(STATUS \"Formatting file: ${_f}\")\n")
        # Gọi clang-format cho file đó
        file(APPEND "${FORMAT_SCRIPT}" "execute_process(COMMAND \"${CLANG_FORMAT}\" -i --style=file:${CLANG_FORMAT_FILE} \"${_f}\")\n")
      endforeach()

      # Tạo target format
      add_custom_target(clang_format
        COMMAND "${CMAKE_COMMAND}" -P "${FORMAT_SCRIPT}"
        COMMENT "Running clang-format on M0_SysApp"
      )
    endif()
  else()
    message(STATUS "M0_SysApp sources not available at configure time; skip clang_format target.")
  endif()
endif()

# ===========================
# Host tests (native) - chỉ thêm khi BUILD_HOST_TESTS=ON
# ===========================
if(BUILD_HOST_TESTS)
  enable_testing()
  add_subdirectory(test)   # Thư mục test của bạn (KHÔNG được dùng flag ARM trong đó)
endif()
