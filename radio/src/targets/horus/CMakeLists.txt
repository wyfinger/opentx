option(DISK_CACHE "Enable SD card disk cache" ON)
option(UNEXPECTED_SHUTDOWN "Enable the Unexpected Shutdown screen" ON)
option(PXX1 "PXX1 protocol support" ON)
option(PXX2 "PXX2 protocol support" OFF)
option(AFHDS3 "AFHDS3 TX Module" ON)
option(MULTIMODULE "DIY Multiprotocol TX Module (https://github.com/pascallanger/DIY-Multiprotocol-TX-Module)" ON)
option(GHOST "Ghost TX Module" ON)
option(HARDWARE_EXTERNAL_ACCESS_MOD "Support for R9M 2019 hardware mod" OFF)

set(PWR_BUTTON "PRESS" CACHE STRING "Pwr button type (PRESS/SWITCH)")
set(CPU_TYPE STM32F4)
set(HSE_VALUE 12000000)
set(SDCARD YES)
set(EEPROM SDCARD)
set(HAPTIC YES)
set(GUI_DIR 480x272)
set(NAVIGATION_TYPE horus)
set(TARGET_DIR horus)
set(LINKER_SCRIPT targets/horus/stm32f4_flash.ld)
set(RTC_BACKUP_RAM YES)
set(PPM_LIMITS_SYMETRICAL YES)
set(USB_SERIAL ON CACHE BOOL "Enable USB serial (CDC)")
set(ROTARY_ENCODER YES)
set(EXTERNAL_MODULE YES)
add_definitions(-DHARDWARE_TRIMS)

# for size report script
set(CPU_TYPE_FULL STM32F429xI)
set(SIZE_TARGET_MEM_DEFINE "MEM_SIZE_SDRAM2=8192")

if(SPLASH STREQUAL OFF)
  message(FATAL_ERROR "Option SPLASH=OFF not supported on Horus")
endif()

if (PCB STREQUAL X10)
  set(PCBREV "STD" CACHE STRING "PCB Revision")
  add_definitions(-DPCBX10)
  add_definitions(-DSOFTWARE_VOLUME)
  set(TARGET_SRC
    ${TARGET_SRC}
    ../common/arm/stm32/audio_dac_driver.cpp
    ../common/arm/stm32/adc_driver.cpp
    ../common/arm/stm32/sticks_pwm_driver.cpp
    )
  set(BITMAPS_TARGET x10_bitmaps)
  set(FONTS_TARGET x10_fonts)
  set(LCD_DRIVER lcd_driver.cpp)
  set(LUA_EXPORT lua_export_x10)
  set(INTERNAL_GPS OFF)
  if (PCBREV STREQUAL EXPRESS)
    set(FLAVOUR x10express)
    option(INTERNAL_MODULE_PXX1 "Support for PXX1 internal module" OFF)
    option(INTERNAL_MODULE_PXX2 "Support for PXX2 internal module" ON)
    set(BLUETOOTH ON)
    add_definitions(-DHARDWARE_POWER_MANAGEMENT_UNIT)
    add_definitions(-DRADIO_X10E)
    add_definitions(-DMANUFACTURER_FRSKY)
  elseif (PCBREV STREQUAL T16)
    set(FLAVOUR t16)
    set(LUA_EXPORT lua_export_t16)
    add_definitions(-DRADIO_T16)
    add_definitions(-DRADIO_FAMILY_T16)
    option(INTERNAL_MODULE_MULTI "Support for MULTI internal module" OFF)
    option(BLUETOOTH "Support for bluetooth module" OFF)
    add_definitions(-DMANUFACTURER_JUMPER)
  elseif (PCBREV STREQUAL TX16S)
    set(FLAVOUR tx16s)
    set(LUA_EXPORT lua_export_t16)
    add_definitions(-DRADIO_TX16S)
    add_definitions(-DRADIO_FAMILY_T16)
    option(INTERNAL_MODULE_MULTI "Support for MULTI internal module" ON)
    option(BLUETOOTH "Support for bluetooth module" OFF)
    option(INTERNAL_GPS "Support for internal ublox GPS" OFF)
    set(HARDWARE_TOUCH YES)
    set(USB_CHARGER YES)
    set(DEFAULT_THEME "DARKBLUE")
    set(SBUS_TRAINER ON)
    set(AUX_SERIAL ON)
    add_definitions(-DMANUFACTURER_RADIOMASTER)
    if (NOT BLUETOOTH AND NOT INTERNAL_GPS)
      set(AUX2_SERIAL ON)
    endif()
    set(INTERNAL_GPS_BAUDRATE "9600" CACHE STRING "Baud rate for internal GPS")
  elseif (PCBREV STREQUAL T18)
    set(FLAVOUR t18)
    set(LUA_EXPORT lua_export_t16)
    add_definitions(-DRADIO_T18)
    add_definitions(-DRADIO_FAMILY_T16)
    option(INTERNAL_MODULE_MULTI "Support for MULTI internal module" ON)
    option(BLUETOOTH "Support for bluetooth module" OFF)
    add_definitions(-DMANUFACTURER_JUMPER)
    set(HARDWARE_TOUCH YES)
  else()
    set(FLAVOUR x10)
    option(INTERNAL_MODULE_PXX1 "Support for PXX1 internal module" ON)
    option(INTERNAL_MODULE_PXX2 "Support for PXX2 internal module" OFF)
    set(BLUETOOTH ON)
    add_definitions(-DMANUFACTURER_FRSKY)
  endif()
elseif (PCB STREQUAL X12S)
  option(INTERNAL_MODULE_PXX1 "Support for PXX1 internal module" ON)
  option(INTERNAL_MODULE_PXX2 "Support for PXX2 internal module" OFF)
  set(FLAVOUR x12s)
  set(BLUETOOTH ON)
  set(PCBREV "13" CACHE STRING "PCB Revision")
  if(${PCBREV} GREATER 10)
    option(INTERNAL_GPS "Internal GPS installed" YES)
  else()
    option(INTERNAL_GPS "Internal GPS installed" NO)
    if(NOT INTERNAL_GPS)
      message("Horus: Internal GPS is optional, use INTERNAL_GPS=YES option to enable it")
    endif()
  endif()
  set(INTERNAL_GPS_BAUDRATE "9600" CACHE STRING "Baud rate for internal GPS")
  add_definitions(-DPCBX12S)
  set(TARGET_SRC
    ${TARGET_SRC}
    audio_spi_driver.cpp
    x12s_adc_driver.cpp
    )
  set(BITMAPS_TARGET x12s_bitmaps)
  set(FONTS_TARGET x12s_fonts)
  set(LCD_DRIVER lcd_driver.cpp)
  set(LUA_EXPORT lua_export_x12s)
  add_definitions(-DMANUFACTURER_FRSKY)
endif()

add_definitions(-DPCBREV=${PCBREV})
add_definitions(-DPCBREV_${PCBREV})

set(RADIO_DEPENDENCIES
  ${RADIO_DEPENDENCIES}
  ${BITMAPS_TARGET}
  )

set(FIRMWARE_DEPENDENCIES datacopy)

add_definitions(-DPCBHORUS -DSTM32F429_439xx -DSDRAM -DCOLORLCD)
add_definitions(-DEEPROM_VARIANT=0 -DAUDIO -DVOICE -DRTCLOCK)
add_definitions(-DGPS_USART_BAUDRATE=${INTERNAL_GPS_BAUDRATE})
add_definitions(-DPWR_BUTTON_${PWR_BUTTON})

if(DEFAULT_THEME)
  add_definitions(-DDEFAULT_THEME_${DEFAULT_THEME})
else()
  add_definitions(-DDEFAULT_THEME_DEFAULT)
endif()

if(NOT UNEXPECTED_SHUTDOWN)
  add_definitions(-DNO_UNEXPECTED_SHUTDOWN)
endif()

if(HARDWARE_EXTERNAL_ACCESS_MOD)
  set(PXX2 ON)
  set(AUX_SERIAL OFF)
  add_definitions(-DHARDWARE_EXTERNAL_ACCESS_MOD)
endif()

if(INTERNAL_MODULE_PXX1)
  add_definitions(-DHARDWARE_INTERNAL_MODULE)
  add_definitions(-DINTERNAL_MODULE_PXX1)
endif()

if(INTERNAL_MODULE_PXX2)
  set(PXX2 ON)
  add_definitions(-DHARDWARE_INTERNAL_MODULE)
  add_definitions(-DINTERNAL_MODULE_PXX2)
  if (NOT PCBREV STREQUAL EXPRESS)
    set(ACCESS_LIB ON)
    add_definitions(-DACCESS_LIB)
  endif()
endif()

if(INTERNAL_MODULE_MULTI)
  add_definitions(-DHARDWARE_INTERNAL_MODULE)
  add_definitions(-DINTERNAL_MODULE_MULTI)
endif()

if(EXTERNAL_MODULE)
  add_definitions(-DHARDWARE_EXTERNAL_MODULE)
endif()

include_directories(${RADIO_SRC_DIRECTORY}/fonts/480x272 gui/${GUI_DIR} gui/${GUI_DIR}/layouts)

file(GLOB THEMES_SRC RELATIVE ${RADIO_SRC_DIRECTORY}/gui/480x272 ${RADIO_SRC_DIRECTORY}/gui/480x272/themes/*.cpp)
file(GLOB LAYOUTS_SRC RELATIVE ${RADIO_SRC_DIRECTORY}/gui/480x272 ${RADIO_SRC_DIRECTORY}/gui/480x272/layouts/*.cpp)
file(GLOB WIDGETS_SRC RELATIVE ${RADIO_SRC_DIRECTORY}/gui/480x272 ${RADIO_SRC_DIRECTORY}/gui/480x272/widgets/*.cpp)
set(GUI_SRC
  ${GUI_SRC}
  bitmapbuffer.cpp
  curves.cpp
  bitmaps.cpp
  radio_sdmanager.cpp
  radio_hardware.cpp
  radio_diagkeys.cpp
  radio_diaganas.cpp
  model_inputs.cpp
  model_mixes.cpp
  model_curves.cpp
  view_channels.cpp
  view_logical_switches.cpp
  view_about.cpp
  view_text.cpp
  screens_setup.cpp
  theme.cpp
  topbar.cpp
  layout.cpp
  widget.cpp
  menus.cpp
  ${THEMES_SRC}
  ${LAYOUTS_SRC}
  ${WIDGETS_SRC}
  rle.cpp
  )

if(PXX2 OR LUA OR MULTIMODULE)
  set(GUI_SRC ${GUI_SRC} radio_tools.cpp)
endif()

if(PXX2 OR MULTIMODULE)
  set(GUI_SRC ${GUI_SRC} radio_spectrum_analyser.cpp)
endif()

if(PXX2)
  set(GUI_SRC ${GUI_SRC} radio_power_meter.cpp)
endif()

if(GHOST)
  set(GUI_SRC ${GUI_SRC} radio_ghost_menu.cpp)
endif()

if(DISK_CACHE)
  set(SRC ${SRC} disk_cache.cpp)
  add_definitions(-DDISK_CACHE)
endif()

if(INTERNAL_GPS)
  set(SRC ${SRC} gps.cpp)
  set(TARGET_SRC ${TARGET_SRC} gps_driver.cpp)
  add_definitions(-DINTERNAL_GPS)
  message("Horus: Internal GPS enabled")
endif()

set(GVAR_SCREEN model_gvars.cpp)

set(TARGET_SRC
  ${TARGET_SRC}
  led_driver.cpp
  extmodule_driver.cpp
  trainer_driver.cpp
  ../common/arm/stm32/mixer_scheduler_driver.cpp
  ../common/arm/stm32/heartbeat_driver.cpp
  ../common/arm/stm32/timers_driver.cpp
  ../common/arm/stm32/intmodule_serial_driver.cpp
  ../common/arm/stm32/rotary_encoder_driver.cpp
  )

if(HARDWARE_TOUCH)
  set(TARGET_SRC ${TARGET_SRC}
    tp_gt911.cpp
    )
  add_definitions(-DHARDWARE_TOUCH)
endif()

if(USB_CHARGER)
  set(TARGET_SRC ${TARGET_SRC} usb_charger_driver.cpp)
  add_definitions(-DUSB_CHARGER)
endif()

if(BLUETOOTH)
  add_definitions(-DBLUETOOTH)
  set(TARGET_SRC ${TARGET_SRC} ../common/arm/stm32/bluetooth_driver.cpp)
  set(SRC ${SRC} bluetooth.cpp)
endif()

if (AUX_SERIAL)
  add_definitions(-DAUX_SERIAL)
  set(AUX_SERIAL_DRIVER ../common/arm/stm32/aux_serial_driver.cpp)
endif()

if (AUX2_SERIAL)
  add_definitions(-DAUX2_SERIAL)
  set(AUX_SERIAL_DRIVER ../common/arm/stm32/aux_serial_driver.cpp)
endif()

if (SBUS_TRAINER)
  add_definitions(-DSBUS_TRAINER)
  set(SRC
    ${SRC}
    sbus.cpp
    )
endif()

set(FIRMWARE_TARGET_SRC
  ${FIRMWARE_TARGET_SRC}
  ${LCD_DRIVER}
  ${AUX_SERIAL_DRIVER}
  board.cpp
  backlight_driver.cpp
  ../common/arm/stm32/pwr_driver.cpp
  sdram_driver.c
  startup_stm32f42_43xxx.s
  ../common/arm/stm32/sdio_sd.c
  ../common/arm/loadboot.cpp
  )

set(SRC
  ${SRC}
  io/frsky_firmware_update.cpp
  io/multi_firmware_update.cpp
  )

set(STM32LIB_SRC
  STM32F4xx_StdPeriph_Driver/src/stm32f4xx_sdio.c
  STM32F4xx_StdPeriph_Driver/src/stm32f4xx_fmc.c
  STM32F4xx_StdPeriph_Driver/src/stm32f4xx_ltdc.c
  STM32F4xx_StdPeriph_Driver/src/stm32f4xx_tim.c
  STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dma2d.c
  )

MACRO(GET_GCC_INCLUDE_PATH gcc_include_path)
  if (WIN32)
    execute_process(COMMAND arm-none-eabi-gcc -v -x c -E NUL ERROR_VARIABLE _gcc_output OUTPUT_QUIET)
  else()
    execute_process(COMMAND arm-none-eabi-gcc -v -x c -E - INPUT_FILE /dev/null ERROR_VARIABLE _gcc_output OUTPUT_QUIET)
  endif()

  # Build an array of string from the GCC output
  string(REPLACE "\n" ";" _gcc_output "${_gcc_output}")

  set(_capture_include FALSE)
  set(_include_path "")

  # Go through the lines and capture between '"#include <...> search starts here:"' and 'End of search list.'
  foreach(_line ${_gcc_output})
    if(${_line} STREQUAL "End of search list.")
      set(_capture_include FALSE)
    endif()

    if(_capture_include)
      # Remove the leading and trailing empty characters
      string(REPLACE "\r" "" _line ${_line})
      string(SUBSTRING "${_line}" 1 -1 _line)
      set(_include_path "${_include_path}" "-I${_line}")
    endif()

    if(${_line} STREQUAL "#include <...> search starts here:")
      set(_capture_include TRUE)
    endif()
  endforeach()
  set(${gcc_include_path} ${_include_path})
ENDMACRO()

if(PYTHONINTERP_FOUND)
  GET_GCC_INCLUDE_PATH(arm_gcc_include_path)

  add_custom_command(
    OUTPUT ${PROJECT_BINARY_DIR}/radio/src/datacopy.cpp
    WORKING_DIRECTORY ${RADIO_DIRECTORY}/src
    COMMAND ${PYTHON_EXECUTABLE} ${RADIO_DIRECTORY}/util/generate_datacopy.py datastructs.h -DPCBHORUS -DPCBX10 -DCPUARM -DSTM32F4 -DSTM32F429_439xx -DCOLORLCD -DBACKUP -DSIMU -I. -Itargets/horus -Itargets/common/arm/stm32 -I${THIRDPARTY_DIR} -I${THIRDPARTY_DIR}/STM32F4xx_DSP_StdPeriph_Lib_V1.4.0/Libraries/CMSIS/Device/ST/STM32F4xx/Include ${arm_gcc_include_path} > ${PROJECT_BINARY_DIR}/radio/src/datacopy.cpp
    DEPENDS ${RADIO_DIRECTORY}/src/datastructs.h ${RADIO_DIRECTORY}/util/generate_datacopy.py
    )
  add_custom_target(datacopy
    DEPENDS ${PROJECT_BINARY_DIR}/radio/src/datacopy.cpp
    )
endif()
