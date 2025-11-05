cmake_minimum_required(VERSION 3.20)

project(MySTM32App C ASM)

# Khai báo target firmware dùng toàn project
set(FW_TARGET firmware CACHE STRING "Firmware target name")

# Tạo target + cấu hình toolchain/linker ở ide_gen
add_subdirectory(ide_gen)

# Bơm mã nguồn ứng dụng vào target đã tạo
add_subdirectory(src/M0_SysApp)
