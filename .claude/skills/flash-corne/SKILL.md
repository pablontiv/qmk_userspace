---
name: flash-corne
description: "Trigger: flash, flashear, flash corne, flash keyboard, bootloader, avrdude, qmk flash. Compile and flash the Corne (crkbd/rev1:pones) on macOS safely."
license: Apache-2.0
metadata:
  author: "pones"
  version: "1.0"
---

## Activation Contract

Load when the user asks to flash the Corne, update keyboard firmware, or debug a failed flash on macOS in this repo.

## Hard Rules

- NEVER use `qmk flash` for the flash step on macOS: its detector matches pseudo-terminals and flashes garbage (see references/gotchas.md).
- Flash only ports matching `/dev/cu.usbmodem*` that appear AFTER the user presses RESET; pre-existing usbmodem ports belong to other devices.
- The physical RESET button is the only bootloader entry: the firmware is pure HID (no serial, no QK_BOOT). Do not attempt 1200-baud resets.
- Do not enable features in `config.h`/`rules.mk` to "fix" a build without checking the 28672-byte size limit.

## Decision Gates

| Situation | Action |
|-----------|--------|
| Firmware over size limit | Read references/gotchas.md "Firmware size limit" |
| avrdude died mid-write | Board is recoverable — press RESET, re-run script |
| `qmk doctor` fails | Read references/gotchas.md "Toolchain" |
| Both halves at once | Both need own USB cable; RESET one at a time |

## Execution Steps

1. Compile: `qmk compile -kb crkbd/rev1 -km pones`. Confirm size warning stays under 28672 bytes.
2. Run `.claude/skills/flash-corne/assets/flash-macos.sh [halves]` (default 2).
3. Tell the user to press RESET on one half; script flashes it, then repeats for the next.
4. Verify: `ioreg -p IOUSB -w0 | rg '\+\-o' | rg -i corne` shows the half re-enumerated as "Corne".

## Output Contract

Report: compile size (bytes/limit), each half's flash result (verified bytes), and re-enumeration check. On failure, quote the avrdude error and the matching gotcha.

## References

- `assets/flash-macos.sh` — proven watcher + avrdude flash script.
- `references/gotchas.md` — failure modes: pty race, wrong ports, size limit, recovery, toolchain setup.
