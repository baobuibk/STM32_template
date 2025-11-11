# === OSAL sources merged into firmware target ===
set(OSAL_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/freertos/osal.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/freertos/osal_log.c
)
target_include_directories(${FW_TARGET}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sys
)
# Apply strict warnings only to OSAL files (no extra target created)
foreach(_f IN LISTS OSAL_SOURCES)
  set_source_files_properties(${_f} PROPERTIES
    COMPILE_OPTIONS "-Wall;-Wextra;-Werror;-Wno-unused-parameter"
  )
endforeach()

# Add include path for FreeRTOS headers to the firmware
set(_OSAL_INCLUDE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/external/freeRTOS>)

# Attach to firmware target if provided; else export to parent for later use
if(DEFINED FW_TARGET AND TARGET ${FW_TARGET})
  target_sources(${FW_TARGET} PRIVATE ${OSAL_SOURCES})
  target_include_directories(${FW_TARGET} PRIVATE ${_OSAL_INCLUDE})
elseif(DEFINED FW_TARGET AND NOT TARGET ${FW_TARGET})
  message(FATAL_ERROR "FW_TARGET='${FW_TARGET}' is defined but target not found at this point. Include FREERTOS.cmake after defining the firmware target.")
else()
  # Fallback: propagate variables so parent CMakeLists can attach them
  set(OSAL_SOURCES ${OSAL_SOURCES} PARENT_SCOPE)
  set(OSAL_INCLUDE_DIRS "${_OSAL_INCLUDE}" PARENT_SCOPE)
endif()