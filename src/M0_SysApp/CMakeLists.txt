# Bảo vệ: chắc chắn biến đã có
if(NOT DEFINED FW_TARGET)
  message(FATAL_ERROR "FW_TARGET is not defined. Define it in the root CMakeLists before add_subdirectory(App).")
endif()

# 1) KHAI BÁO DANH SÁCH NGUỒN & HEADER (liệt kê tường minh)
set(M0_SYSAPP_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/Src/main_app.c
  # thêm file .c khác tại đây ...
)

set(M0_SYSAPP_HEADERS
  #${CMAKE_CURRENT_SOURCE_DIR}/Inc/app.h
  # thêm file .h khác tại đây ...
)

# 2) GẮN VÀO TARGET ĐỂ BIÊN DỊCH
target_sources(${FW_TARGET} PRIVATE ${M0_SYSAPP_SOURCES})
target_include_directories(${FW_TARGET} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/Inc
)


# 3) XUẤT BIẾN RA PHẠM VI CHA (để root dùng cho format/lint,...)
set(M0_SYSAPP_SOURCES "${M0_SYSAPP_SOURCES}" PARENT_SCOPE)
set(M0_SYSAPP_HEADERS "${M0_SYSAPP_HEADERS}" PARENT_SCOPE)

# (Tùy chọn) Định nghĩa compile riêng cho App
# target_compile_definitions(firmware.elf PRIVATE APP_FEATURE_X=1)