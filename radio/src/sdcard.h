/*
 * Copyright (C) OpenTX
 *
 * Based on code named
 *   th9x - http://code.google.com/p/th9x
 *   er9x - http://code.google.com/p/er9x
 *   gruvin9x - http://code.google.com/p/gruvin9x
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _SDCARD_H_
#define _SDCARD_H_

#if !defined(SIMU)
#include "ff.h"
#endif

#include "opentx.h"

#define ROOT_PATH           "/"
#define MODELS_PATH         ROOT_PATH "MODELS"      // no trailing slash = important
#define RADIO_PATH          ROOT_PATH "RADIO"       // no trailing slash = important
#define LOGS_PATH           ROOT_PATH "LOGS"
#define SCREENSHOTS_PATH    ROOT_PATH "SCREENSHOTS"
#define SOUNDS_PATH         ROOT_PATH "SOUNDS/en"
#define SOUNDS_PATH_LNG_OFS (sizeof(SOUNDS_PATH)-3)
#define SYSTEM_SUBDIR       "SYSTEM"
#define BITMAPS_PATH        ROOT_PATH "IMAGES"
#define FIRMWARES_PATH      ROOT_PATH "FIRMWARE"
#define AUTOUPDATE_FILENAME FIRMWARES_PATH "/autoupdate.frsk"
#define EEPROMS_PATH        ROOT_PATH "EEPROM"
#define SCRIPTS_PATH        ROOT_PATH "SCRIPTS"
#define WIZARD_PATH         SCRIPTS_PATH "/WIZARD"
#define THEMES_PATH         ROOT_PATH "THEMES"
#define LAYOUTS_PATH        ROOT_PATH "LAYOUTS"
#define WIDGETS_PATH        ROOT_PATH "WIDGETS"
#define WIZARD_NAME         "wizard.lua"
#define SCRIPTS_MIXES_PATH  SCRIPTS_PATH "/MIXES"
#define SCRIPTS_FUNCS_PATH  SCRIPTS_PATH "/FUNCTIONS"
#define SCRIPTS_TELEM_PATH  SCRIPTS_PATH "/TELEMETRY"
#define SCRIPTS_TOOLS_PATH  SCRIPTS_PATH "/TOOLS"

#define LEN_FILE_PATH_MAX   (sizeof(SCRIPTS_TELEM_PATH)+1)  // longest + "/"

#if defined(COLORLCD) || defined(EEPROM_SDCARD)
const char RADIO_MODELSLIST_PATH[] = RADIO_PATH "/models.txt";
const char RADIO_SETTINGS_PATH[] = RADIO_PATH "/radio.bin";
#define    SPLASH_FILE             "splash.png"
#endif

#define MODELS_EXT          ".bin"
#define LOGS_EXT            ".csv"
#define SOUNDS_EXT          ".wav"
#define BMP_EXT             ".bmp"
#define PNG_EXT             ".png"
#define JPG_EXT             ".jpg"
#define SCRIPT_EXT          ".lua"
#define SCRIPT_BIN_EXT      ".luac"
#define TEXT_EXT            ".txt"
#define FIRMWARE_EXT        ".bin"
#define EEPROM_EXT          ".bin"
#define SPORT_FIRMWARE_EXT  ".frk"
#define FRSKY_FIRMWARE_EXT  ".frsk"
#define MULTI_FIRMWARE_EXT  ".bin"
#define ELRS_FIRMWARE_EXT   ".elrs"

#define LEN_FILE_EXTENSION_MAX  5  // longest used, including the dot, excluding null term.

#if defined(PCBHORUS) || defined(EEPROM_SDCARD)
#define BITMAPS_EXT         BMP_EXT JPG_EXT PNG_EXT
#define LEN_BITMAPS_EXT     4
#else
#define BITMAPS_EXT         BMP_EXT
#endif

#ifdef LUA_COMPILER
  #define SCRIPTS_EXT         SCRIPT_BIN_EXT SCRIPT_EXT
#else
  #define SCRIPTS_EXT         SCRIPT_EXT
#endif

#define GET_FILENAME(filename, path, var, ext) \
  char filename[sizeof(path) + sizeof(var) + sizeof(ext)]; \
  memcpy(filename, path, sizeof(path) - 1); \
  filename[sizeof(path) - 1] = '/'; \
  memcpy(&filename[sizeof(path)], var, sizeof(var)); \
  filename[sizeof(path)+sizeof(var)] = '\0'; \
  strcat(&filename[sizeof(path)], ext)

extern FATFS g_FATFS_Obj;
extern FIL g_oLogFile;

extern uint8_t logDelay;
void logsInit();
void logsClose();
void logsWrite();

bool sdCardFormat();
uint32_t sdGetNoSectors();
uint32_t sdGetSize();
uint32_t sdGetFreeSectors();
const char * sdCheckAndCreateDirectory(const char * path);

#if !defined(BOOT)
inline const char * SDCARD_ERROR(FRESULT result)
{
  if (result == FR_NOT_READY)
    return STR_NO_SDCARD;
  else
    return STR_SDCARD_ERROR;
}
#endif

// NOTE: 'size' must = 0 or be a valid character position within 'filename' array -- it is NOT validated
const char * getFileExtension(const char * filename, uint8_t size=0, uint8_t extMaxLen=0, uint8_t * fnlen=nullptr, uint8_t * extlen=nullptr);
const char * getBasename(const char * path);

#if defined(PCBX12S)
  #define OTX_FOURCC 0x3478746F // otx for X12S
#elif defined(RADIO_T16)
  #define OTX_FOURCC 0x3F78746F // otx for Jumper T16
#elif defined(RADIO_T18)
  #define OTX_FOURCC 0x4078746F // otx for Jumper T18
#elif defined(RADIO_TX16S)
  #define OTX_FOURCC 0x3878746F // otx for Radiomaster TX16S
#elif defined(PCBX10)
  #define OTX_FOURCC 0x3778746F // otx for X10
#elif defined(PCBX9E)
  #define OTX_FOURCC 0x3578746F // otx for Taranis X9E
#elif defined(RADIO_TANGO)
  #define OTX_FOURCC 0x4478746F // otx for Tango
#elif defined(RADIO_MAMBO)
  #define OTX_FOURCC 0x4578746F // otx for Mambo
#elif defined(PCBXLITES)
  #define OTX_FOURCC 0x3B78746F // otx for Taranis X-Lite S
#elif defined(PCBXLITE)
  #define OTX_FOURCC 0x3978746F // otx for Taranis X-Lite
#elif defined(RADIO_T12)
  #define OTX_FOURCC 0x3D78746F // otx for Jumper T12
#elif defined(RADIO_TLITE)
  #define OTX_FOURCC 0x4278746F // otx for Jumper TLite
#elif defined(RADIO_TX12)
  #define OTX_FOURCC 0x4178746F // otx for Radiomaster TX12
#elif defined(RADIO_T8)
  #define OTX_FOURCC 0x4378746F // otx for Radiomaster T8
#elif defined(PCBX7)
  #define OTX_FOURCC 0x3678746F // otx for Taranis X7 / X7S / X7 Express / X7S Express
#elif defined(PCBX9LITES)
  #define OTX_FOURCC 0x3E78746F // otx for Taranis X9-Lite S
#elif defined(PCBX9LITE)
  #define OTX_FOURCC 0x3C78746F // otx for Taranis X9-Lite
#elif defined(PCBX9D) || defined(PCBX9DP)
  #define OTX_FOURCC 0x3378746F // otx for Taranis X9D
#elif defined(PCBSKY9X)
  #define OTX_FOURCC 0x3278746F // otx for sky9x
#endif

bool isFileAvailable(const char * filename, bool exclDir = false);
int findNextFileIndex(char * filename, uint8_t size, const char * directory);
bool isExtensionMatching(const char * extension, const char * pattern, char * match = nullptr);

const char * sdCopyFile(const char * src, const char * dest);
const char * sdCopyFile(const char * srcFilename, const char * srcDir, const char * destFilename, const char * destDir);

#define LIST_NONE_SD_FILE   1
#define LIST_SD_FILE_EXT    2
bool sdListFiles(const char * path, const char * extension, const uint8_t maxlen, const char * selection, uint8_t flags=0);

void sdReadTextFile(const char * filename, char lines[NUM_BODY_LINES][LCD_COLS + 1], int & lines_count);

bool isCwdAtRoot();
FRESULT sdReadDir(DIR * dir, FILINFO * fno, bool & firstTime);

#endif // _SDCARD_H_
