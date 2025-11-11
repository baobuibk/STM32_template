cmake_minimum_required (VERSION 3.14)

set(OSAL_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/tools")

# Always use standard .o suffix
set(CMAKE_C_OUTPUT_EXTENSION_REPLACE 1)
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)



# Suppress certain warnings when building with MSVC
if (WINDOWS_MONO)
target_compile_options (osal
  PRIVATE
  /wd4820 # padding added
  )
endif()

# Use position independent code if platform supports shared libraries
get_property(SUPPORTS_SHARED GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS)
if (SUPPORTS_SHARED)
  set_property(TARGET osal PROPERTY POSITION_INDEPENDENT_CODE ON)
endif()

# Platform configuration
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/${SYS_OS}.cmake)
target_sources(${PROJECT_NAME} PRIVATE 
  ${OSAL_SOURCES}
  ${FREE_RTOS_SOURCES}
)

target_include_directories(${PROJECT_NAME}
  PRIVATE
    ${OSAL_INCLUDE}
    ${FREE_RTOS_INCLUDE}
)





