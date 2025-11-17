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



# Platform configuration
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/${SYS_OS}.cmake)






