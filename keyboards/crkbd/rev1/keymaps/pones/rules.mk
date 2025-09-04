AUTO_SHIFT_ENABLE = no
LTO_ENABLE = yes
NKRO_ENABLE = yes
CAPS_WORD_ENABLE = yes
VIA_ENABLE      = no
DYNAMIC_TAPPING_TERM_ENABLE = no
RGBLIGHT_ENABLE    = no
RGB_MATRIX_ENABLE  = yes
OLED_ENABLE        = yes
CONSOLE_ENABLE = no
COMMAND_ENABLE = no
MOUSEKEY_ENABLE = no
SPACE_CADET_ENABLE = no
GRAVE_ESC_ENABLE = no
MAGIC_ENABLE = no
COMBO_ENABLE = yes
TAP_DANCE_ENABLE = yes
SPACE_CADET_ENABLE = no

DEBOUNCE_TYPE = asym_eager_defer_pk

# Include feature source files
SRC += features/tap_dance.c
SRC += features/macros.c

ifeq ($(strip $(OLED_ENABLE)), yes)
    SRC += features/oled.c
endif
