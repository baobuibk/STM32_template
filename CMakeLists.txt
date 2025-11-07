cmake_minimum_required(VERSION 3.20)

project(MySTM32App C ASM)

# Khai báo target firmware dùng toàn project
set(FW_TARGET firmware CACHE STRING "Firmware target name")

# Tạo target + cấu hình toolchain/linker ở ide_gen
add_subdirectory(ide_gen)

# Bơm mã nguồn ứng dụng vào target đã tạo
add_subdirectory(src/M0_SysApp)

# Đường dẫn đến file .clang-format
set(CLANG_FORMAT_FILE "${CMAKE_SOURCE_DIR}/tools/.clang-format")

set(FORMAT_SOURCES
    ${M0_SYSAPP_SOURCES}
    ${M0_SYSAPP_HEADERS}
)

# Kiểm tra có file nào không
if(NOT FORMAT_SOURCES)
    message(WARNING "No files found in SOURCES or HEADERS")
endif()

# Tìm clang-format
find_program(CLANG_FORMAT NAMES clang-format REQUIRED)

# Tạo script để chạy lần lượt (ổn định cả Windows & Linux)
set(FORMAT_SCRIPT "${CMAKE_BINARY_DIR}/cmake/format_m0_sysapp.cmake")
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/cmake")

file(WRITE "${FORMAT_SCRIPT}" "message(STATUS \"Formatting M0_SysApp using ${CLANG_FORMAT_FILE}\")\n")
foreach(_f IN LISTS FORMAT_SOURCES)
  # In ra tên file đang format
  file(APPEND "${FORMAT_SCRIPT}"
       "message(STATUS \"Formatting file: ${_f}\")\n")
  # Gọi clang-format cho file đó
  file(APPEND "${FORMAT_SCRIPT}"
       "execute_process(COMMAND \"${CLANG_FORMAT}\" -i --style=file:${CLANG_FORMAT_FILE} \"${_f}\")\n")
endforeach()

# Tạo target format_m0_sysapp
add_custom_target(clang_format
  COMMAND "${CMAKE_COMMAND}" -P "${FORMAT_SCRIPT}"
  COMMENT "Running clang-format "
)

# Enable testing với CTest
enable_testing()

# Thêm subdirectory cho folder test
add_subdirectory(test)