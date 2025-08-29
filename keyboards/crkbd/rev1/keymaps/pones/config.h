#pragma once

#define MASTER_LEFT

#define LAYER_STATE_16BIT

#define TAPPING_TERM 200

#ifdef OLED_ENABLE
#   define SPLIT_LAYER_STATE_ENABLE
#   define SPLIT_LED_STATE_ENABLE
#   define SPLIT_MODS_ENABLE
#   define SPLIT_OLED_ENABLE
#   define OLED_FONT_H "keyboards/crkbd/rev1/keymaps/pones/glcdfont.c"
#   define OLED_TIMEOUT 30000
#endif

#define DYNAMIC_KEYMAP_LAYER_COUNT 10

// Tap dance configuration
#define TAPPING_TERM_PER_KEY

// Home row mods optimization
#define PERMISSIVE_HOLD
#define QUICK_TAP_TERM 120
#define BILATERAL_COMBINATIONS
#define BILATERAL_COMBINATIONS_TYPING_STREAK_TIMEOUT 160
#define COMBO_TERM 30

