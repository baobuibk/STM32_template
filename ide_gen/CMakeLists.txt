cmake_minimum_required(VERSION 3.20)
project(STM32F746_FW C ASM)

# ===========================
# 0) Cấu hình target & output (dễ đổi)
# ===========================
# Nếu root đã set(FW_TARGET ... CACHE STRING ...), đoạn dưới sẽ giữ nguyên; nếu chưa, đặt mặc định là "firmware"
if(NOT DEFINED FW_TARGET)
  set(FW_TARGET firmware CACHE STRING "Firmware target name")
endif()

# Thư mục đặt file .elf/.hex/.bin và .map (mặc định: build dir hiện tại)
set(TARGET_OUT_DIR "${CMAKE_BINARY_DIR}" CACHE PATH "Output directory for firmware files")
set(MAP_FILE "${TARGET_OUT_DIR}/${FW_TARGET}.map")
set(HEX_FILE "${TARGET_OUT_DIR}/${FW_TARGET}.hex")
set(BIN_FILE "${TARGET_OUT_DIR}/${FW_TARGET}.bin")

# Nếu muốn ép phần mở rộng thực thi là .elf (tuỳ IDE), bỏ comment dòng sau
# set(CMAKE_EXECUTABLE_SUFFIX ".elf")

# ===========================
# 1) Thiết lập C / MCU
# ===========================
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS ON)

# Cortex-M7 + FPU (hard ABI) -> mỗi flag là 1 phần tử
set(CPU_FLAGS
  -mcpu=cortex-m7
  -mthumb
  -mfpu=fpv5-d16
  -mfloat-abi=hard
)

# ===========================
# 2) Linker script
# ===========================
set(_LDS_1 ${CMAKE_CURRENT_SOURCE_DIR}/STM32F746NGHX_FLASH.ld)
set(_LDS_2 ${CMAKE_CURRENT_SOURCE_DIR}/../STM32F746NGHX_FLASH.ld)
if (EXISTS ${_LDS_1})
  set(LINKER_SCRIPT ${_LDS_1})
elseif (EXISTS ${_LDS_2})
  set(LINKER_SCRIPT ${_LDS_2})
else()
  message(FATAL_ERROR "Linker script STM32F746NGHX_FLASH.ld not found in ide_gen/ or parent folder")
endif()

# ===========================
# 3) Include directories
# ===========================
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc
  ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Inc
  ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Include
  ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F7xx/Include
)

# ===========================
# 4) Sources
#    (Nếu main.c nằm ở App/, hãy xoá dòng main.c trong SRC_CORE)
# ===========================
set(SRC_CORE
  ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/main.c
  ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/stm32f7xx_hal_msp.c
  ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/stm32f7xx_it.c
  ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/syscalls.c
  ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/sysmem.c
  ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/system_stm32f7xx.c
  ${CMAKE_CURRENT_SOURCE_DIR}/Core/Startup/startup_stm32f746nghx.s
)

set(SRC_HAL
  ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.c
  ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.c
  ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma.c
  ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma_ex.c
  ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_exti.c
  ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash.c
  ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash_ex.c
  ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.c
  ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c.c
  ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c_ex.c
  ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.c
  ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.c
  ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.c
  ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc_ex.c
)

# ===========================
# 5) Executable target
# ===========================
add_executable(${FW_TARGET}
  ${SRC_CORE}
  ${SRC_HAL}
  # App/ sẽ bổ sung thêm nguồn bằng target_sources(${FW_TARGET}) trong App/CMakeLists.txt
)

# Thư mục output cho file thực thi (.elf)
set_target_properties(${FW_TARGET} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${TARGET_OUT_DIR}"
)

# ===========================
# 6) Compile / Link options & defines
# ===========================
target_compile_definitions(${FW_TARGET} PRIVATE
  USE_HAL_DRIVER
  STM32F746xx
)

target_compile_options(${FW_TARGET} PRIVATE
  ${CPU_FLAGS}
  -ffunction-sections
  -fdata-sections
  -fno-builtin
  -g
)

target_link_options(${FW_TARGET} PRIVATE
  ${CPU_FLAGS}
  -Wl,--gc-sections
  -Wl,-Map=${MAP_FILE}
  -T${LINKER_SCRIPT}
)

# ===========================
# 7) Post-build: in size + xuất HEX/BIN
# ===========================
find_program(SIZE_TOOL arm-none-eabi-size)
if(SIZE_TOOL)
  add_custom_command(TARGET ${FW_TARGET} POST_BUILD
    COMMAND ${SIZE_TOOL} --format=berkeley $<TARGET_FILE:${FW_TARGET}>
    COMMENT "Size of $<TARGET_FILE_NAME:${FW_TARGET}>"
  )
else()
  message(WARNING "arm-none-eabi-size not found; skipping size print.")
endif()

find_program(OBJCOPY_TOOL arm-none-eabi-objcopy)
if(OBJCOPY_TOOL)
  add_custom_command(TARGET ${FW_TARGET} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${TARGET_OUT_DIR}"
    COMMAND ${OBJCOPY_TOOL} -O ihex   $<TARGET_FILE:${FW_TARGET}> "${HEX_FILE}"
    COMMAND ${OBJCOPY_TOOL} -O binary $<TARGET_FILE:${FW_TARGET}> "${BIN_FILE}"
    COMMENT "Generating HEX and BIN at ${TARGET_OUT_DIR}"
  )
else()
  message(WARNING "arm-none-eabi-objcopy not found; skipping hex/bin generation.")
endif()

# ===========================
# 8) Công bố lại tên target ở dạng cache (để App/ có thể lấy dùng)
# ===========================
set(FW_TARGET "${FW_TARGET}" CACHE STRING "Firmware target name" FORCE)
