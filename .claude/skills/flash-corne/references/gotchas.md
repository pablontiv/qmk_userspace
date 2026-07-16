# Corne flashing on macOS — known failure modes

## `qmk flash` pty race
The QMK caterina detector watches all of `/dev/tty*`. On macOS any new
pseudo-terminal (opening a terminal tab, a tmux pane) matches first, and
avrdude tries to flash `/dev/ttysNNN`. Symptom: `butterfly_recv ... failed`,
`initialization failed (rc = -1)` immediately after "Device /dev/ttysNNN has
appeared". Fix: never use `qmk flash` for the flash step; use the watcher
script, which only matches `/dev/cu.usbmodem*` ports that appeared AFTER the
snapshot.

## Pre-existing usbmodem ports are not the keyboard
Other USB devices can expose CDC serial ports (on this machine the Sound
Blaster X4 owns `/dev/cu.usbmodem21102`). The running QMK firmware is pure
HID and exposes NO serial port. Any usbmodem port present before RESET is
pressed belongs to something else.

## 1200-baud reset does not work
The 1200-baud open/close trick requires a CDC serial port in the running
firmware. This keymap has none, and `QK_BOOT` was removed from the keymap
(commit b31d24f). The physical RESET button is the only bootloader entry.

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
