# === OSAL sources merged into firmware target ===
set(OSAL_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/freertos/osal.c
)

set(FREERTOS_ROOT ${CMAKE_SOURCE_DIR}/external/FreeRTOS)
set(FREERTOS_PORT_DIR ${FREERTOS_ROOT}/portable/GCC/ARM_CM7/r0p1)
set(FREERTOS_MEMMANG  ${FREERTOS_ROOT}/portable/MemMang/heap_4.c)

set(FREE_RTOS_SOURCES
  ${FREERTOS_ROOT}/tasks.c
  ${FREERTOS_ROOT}/queue.c
  ${FREERTOS_ROOT}/timers.c
  ${FREERTOS_ROOT}/list.c
  ${FREERTOS_ROOT}/event_groups.c
  ${FREERTOS_ROOT}/stream_buffer.c
  ${FREERTOS_PORT_DIR}/port.c
  ${FREERTOS_MEMMANG}
)

# Add include path for FreeRTOS headers to the firmware
set(OSAL_INCLUDE 
  ${CMAKE_SOURCE_DIR}/external/freeRTOS/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/src/freertos/
  ${CMAKE_CURRENT_SOURCE_DIR}/src/freertos/sys
)

set(FREE_RTOS_INCLUDE
  ${FREERTOS_ROOT}/Source/include
  ${FREERTOS_PORT_DIR}
  ${CMAKE_SOURCE_DIR}/src/M0_SysApp/Config          # nơi có FreeRTOSConfig.h
)

target_sources(${PROJECT_NAME} PRIVATE 
  ${OSAL_SOURCES}
  ${FREE_RTOS_SOURCES}
)

target_include_directories(${PROJECT_NAME}
  PRIVATE
    ${OSAL_INCLUDE}
    ${FREE_RTOS_INCLUDE}
)