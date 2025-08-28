#pragma once

#define MASTER_LEFT

#define LAYER_STATE_16BIT

#define TAPPING_TERM 220
  
#ifdef OLED_ENABLE
#   define SPLIT_LAYER_STATE_ENABLE
#   define SPLIT_LED_STATE_ENABLE
#   define SPLIT_MODS_ENABLE
#   define SPLIT_OLED_ENABLE
#   define OLED_FONT_H "keyboards/crkbd/rev1/keymaps/pones/glcdfont.c"
#   define OLED_TIMEOUT 30000
#endif

