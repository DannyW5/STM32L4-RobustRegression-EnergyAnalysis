cmake_minimum_required(VERSION 3.20)

set(CMAKE_TOOLCHAIN_FILE cmake/gcc-arm-none-eabi.cmake)

project(STM32L4-RobustRegression-EnergyAnalysis LANGUAGES CXX C ASM
        VERSION 1.0.0
        DESCRIPTION "STM32L476RG Nucleo Board project aimed at investigating the performance and energy consumption of robust linear regression estimators")

set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Set microcontroller information
set(MCU_FAMILY STM32L4xx)
set(MCU_MODEL STM32L476xx)
set(CPU_PARAMETERS
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    )

set(STARTUP_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/startup_stm32l476rgtx.s)
set(MCU_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/inc/platform/STM32L476RGTX_FLASH.ld)

###############################################################################
set(APP ${CMAKE_PROJECT_NAME})

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

###############################################################################
set(STM32_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/platform
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/${MCU_FAMILY}_HAL_Driver/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/${MCU_FAMILY}_HAL_Driver/Inc/Legacy
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/CMSIS/Device/ST/${MCU_FAMILY}/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/CMSIS/Include)

set(PROJECT_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/inc)

add_executable(${APP})

file(GLOB_RECURSE STM32_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/*.c)
target_sources(${APP} PRIVATE 
    ${STM32_SOURCES}
    ${STARTUP_SCRIPT})

add_subdirectory(src)

target_compile_definitions(${APP} PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    ${MCU_MODEL}
    USE_HAL_DRIVER)

target_include_directories(${APP} SYSTEM PRIVATE
    ${STM32_INCLUDE_DIRECTORIES})

target_include_directories(${APP} PRIVATE
    ${PROJECT_INCLUDE_DIRECTORIES})
    
###############################################################################
target_compile_options(${APP} PRIVATE
${CPU_PARAMETERS}
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wdouble-promotion
    -Wformat=2 -Wformat-truncation
    -Wundef
    -fno-common
    -Wno-unused-parameter
    $<$<COMPILE_LANGUAGE:CXX>:
        -Wconversion # STM libraries!
        -Wno-volatile
        -Wold-style-cast
        -Wuseless-cast
        -Wsuggest-override>
    $<$<CONFIG:Debug>:-Og -g3 -ggdb>
    $<$<CONFIG:Release>:-Og -g0>)

target_link_options(${APP} PRIVATE
    -T${MCU_LINKER_SCRIPT}
    ${CPU_PARAMETERS}
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    --specs=nosys.specs
    -Wl,--start-group
    -lc
    -lm
    -lstdc++
    -Wl,--end-group
    -Wl,--print-memory-usage
    -u _printf_float)

add_custom_command(TARGET ${APP} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${APP}>)

add_custom_command(TARGET ${APP} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${APP}> ${APP}.hex
    COMMENT "Generating hex file...")

add_custom_command(TARGET ${APP} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${APP}> ${APP}.bin
    COMMENT "Generating bin file...")
    