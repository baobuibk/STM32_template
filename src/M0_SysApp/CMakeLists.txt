cmake_minimum_required(VERSION 3.20)

# 1) KHAI BÁO DANH SÁCH NGUỒN & HEADER (liệt kê tường minh)
set(INCLUDE_PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/App/Inc
  ${CMAKE_CURRENT_SOURCE_DIR}/Config
  # thêm file .c khác tại đây ...
)

set(SRC_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/App/Src/main_app.c
)

# 2) GẮN VÀO TARGET ĐỂ BIÊN DỊCH
target_sources(${PROJECT_NAME} PRIVATE 
  ${SRC_FILES}
)

target_include_directories(${PROJECT_NAME} PUBLIC

  ${INCLUDE_PATHS}
)