AUTO_SHIFT_ENABLE = no
LTO_ENABLE = yes
NKRO_ENABLE = yes
CAPS_WORD_ENABLE = no
VIA_ENABLE      = yes
DYNAMIC_TAPPING_TERM_ENABLE = yes
RGBLIGHT_ENABLE    = no
RGB_MATRIX_ENABLE  = yes
OLED_ENABLE        = no
CONSOLE_ENABLE = no
COMMAND_ENABLE = no
MOUSEKEY_ENABLE = no
SPACE_CADET_ENABLE = no
GRAVE_ESC_ENABLE = no
MAGIC_ENABLE = no
COMBO_ENABLE = yes
TAP_DANCE_ENABLE = yes

# Include feature source files
SRC += tap_dance.c
SRC += macros.c

ifeq ($(strip $(OLED_ENABLE)), yes)
    SRC += oled.c
endif
