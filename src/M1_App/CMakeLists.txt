# Bảo vệ: chắc chắn biến đã có
if(NOT DEFINED FW_TARGET)
  message(FATAL_ERROR "FW_TARGET is not defined. Define it in the root CMakeLists before add_subdirectory(App).")
endif()

# Bổ sung source của App vào target do ide_gen tạo
target_sources(${FW_TARGET} PRIVATE
  # ${CMAKE_CURRENT_SOURCE_DIR}/Src/app_init.c
  # ...
)

# Include của App
target_include_directories(${FW_TARGET} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc
)

# (Tùy chọn) Định nghĩa compile riêng cho App
# target_compile_definitions(firmware.elf PRIVATE APP_FEATURE_X=1)