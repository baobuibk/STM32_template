# === OSAL sources merged into firmware target ===
set(OSAL_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/windows/osal.c
)

# Add include path for FreeRTOS headers to the firmware
set(OSAL_INCLUDE 
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/src/windows/
  ${CMAKE_CURRENT_SOURCE_DIR}/src/windows/sys
)

target_sources(${PROJECT_NAME} PRIVATE 
  ${OSAL_SOURCES}
)

target_include_directories(${PROJECT_NAME}
  PRIVATE
    ${OSAL_INCLUDE}
)