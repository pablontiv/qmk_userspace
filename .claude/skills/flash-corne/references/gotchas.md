# Corne flashing on macOS — known failure modes

## `qmk flash` pty race
The QMK caterina detector watches all of `/dev/tty*`. On macOS any new
pseudo-terminal (opening a terminal tab, a tmux pane) matches first, and
avrdude tries to flash `/dev/ttysNNN`. Symptom: `butterfly_recv ... failed`,
`initialization failed (rc = -1)` immediately after "Device /dev/ttysNNN has
appeared". Fix: never use `qmk flash` for the flash step; use the watcher
script, which only matches `/dev/cu.usbmodem*` ports that appeared AFTER the
snapshot.

## EE_HANDS bootstrap: handedness written to EEPROM
Firmware v6.3 uses `#define EE_HANDS` (config.h), so each half stores its
handedness in EEPROM instead of being hardcoded. **Bootstrap is one-time per
half**: flash left half with `qmk flash -kb crkbd/rev1 -km pones -bl avrdude-split-left`,
then right with `-bl avrdude-split-right`. QMK generates the eeprom handedness
files automatically; if the pty race hits and forces fallback to manual avrdude,
the command is `avrdude -p atmega32u4 -c avr109 -P <port> -U eeprom:w:<eep_file>:i`
where `<eep_file>` is the `.eep` file QMK creates in the build directory for that
half. After bootstrap, both halves carry the firmware + handedness marks, and
QK_BOOT bootloader works on either half regardless of USB placement.

## Pre-existing usbmodem ports are not the keyboard
Other USB devices can expose CDC serial ports (on this machine the Sound
Blaster X4 owns `/dev/cu.usbmodem21102`). The running QMK firmware is HID only
(bootstrapped v6.3 with QK_BOOT) and exposes no CDC serial port. Any usbmodem
port present before bootloader entry (RESET for bootstrap; QK_BOOT for permanent)
belongs to something else.

## QK_BOOT now available (permanent flow only)
As of firmware v6.3, `QK_BOOT` is in the _MEDIA layer (Backspace hold + Q-key,
row 1 column 1). After bootstrap, this is the primary bootloader entry. The
1200-baud open/close trick still does not work (no CDC serial port). Physical
RESET remains as fallback for bootstrap and emergency recovery.

## Interrupted write is recoverable
If avrdude dies mid-write ("Device not configured"), the application
firmware is corrupt but the caterina bootloader survives in protected flash.
The board may then cycle in and out of bootloader on its own; press RESET
again and re-flash. Nothing is bricked.

## Firmware size limit
atmega32u4 limit is 28672 bytes. With VIA + OLED + RGB Matrix the build sits
near 100%. Current trim: `ENABLE_RGB_MATRIX_DIGITAL_RAIN` and
`RGB_MATRIX_FRAMEBUFFER_EFFECTS` commented out in the keymap `config.h`
(user chose to keep VIA over the animation). If a future QMK bump overflows
again, next candidates: disable VIA (~3KB) or drop the remaining animation.

## Toolchain (one-time, already done on this machine)
`brew trust qmk/qmk osx-cross/arm osx-cross/avr`, `brew install qmk/qmk/qmk
dos2unix`, `brew link --force avr-gcc@8 arm-none-eabi-gcc@8
arm-none-eabi-binutils avr-binutils`, then
`qmk config user.overlay_dir=<userspace path>`. Verify with `qmk doctor`.
