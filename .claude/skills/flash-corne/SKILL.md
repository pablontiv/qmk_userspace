---
name: flash-corne
description: "Trigger: flash, flashear, flash corne, flash keyboard, qmk flash, bootloader, avrdude. Compile and flash the Corne (crkbd/rev1:pones) on macOS — plain qmk recipe."
license: Apache-2.0
metadata:
  author: pones
  version: "2.0"
---

# flash-corne

Straightforward `qmk` recipe. No wrapper script.

## Compile (must stay under 28672 bytes)

```
qmk compile -kb crkbd/rev1 -km pones
```

## First flash per half — bootstrap (writes EE_HANDS handedness)

One half at a time, physical RESET. Plug USB into the half being flashed:

```
qmk flash -kb crkbd/rev1 -km pones -bl avrdude-split-left     # left  half, then press RESET
qmk flash -kb crkbd/rev1 -km pones -bl avrdude-split-right    # right half, then press RESET
```

- Press the half's RESET when it prints `reset your controller now`.
- `-bl avrdude-split-left/right` also writes the handedness EEPROM
  (`eeprom-lefthand.eep` / `eeprom-righthand.eep`) — no manual step.
- If the write dies near ~97% (`Device not configured` / `butterfly_recv failed`),
  just press RESET again and re-run the same command. The caterina bootloader
  survives in protected flash — nothing bricks.

## Later flashes — handedness already set

Enter the bootloader with QK_BOOT, then flash normally:

- Hold **Delete** (opens `_MEDIA`) + tap **Q** (top-left) = QK_BOOT.

```
qmk flash -kb crkbd/rev1 -km pones
```

## Verify it came back as a keyboard (not bootloader)

```
ioreg -p IOUSB -w0 | rg '\+\-o' | rg -i corne
```

## Notes

- `/dev/cu.usbmodem21102` is the Sound Blaster X4, not the keyboard — ignore it.
  The running firmware is HID-only and exposes no serial port.
- Split: only the USB half enumerates over USB; the other rides TRRS. After
  bootstrap either half can take the cable.
- Over 28672 bytes after a QMK bump? Drop an RGB animation or VIA (~3 KB).
- Toolchain (one-time, already done): `brew install qmk/qmk/qmk dos2unix`,
  `qmk config user.overlay_dir=<userspace path>`, verify with `qmk doctor`.
