# Bảo vệ: chắc chắn biến đã có
if(NOT DEFINED FW_TARGET)
  message(FATAL_ERROR "FW_TARGET is not defined. Define it in the root CMakeLists before add_subdirectory(App).")
endif()


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/osal)
