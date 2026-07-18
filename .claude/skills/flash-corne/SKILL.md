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
- Flash only ports matching `/dev/cu.usbmodem*` that appear AFTER bootloader entry (via QK_BOOT or RESET); pre-existing usbmodem ports belong to other devices.
- For **bootstrap only** (one-time per half): use `qmk flash -kb crkbd/rev1 -km pones -bl avrdude-split-left` (or `-right` for right half), or fallback to manual avrdude + eeprom handedness write if pty race hits. Physical RESET is required for bootstrap because the pre-v6.3 firmware has no bootloader key.
- For **permanent flow** (all future flashes): enter bootloader via `_MEDIA`-hold (Backspace) + QK_BOOT on the half with USB cable (Q-key position, row 1, column 1 of _MEDIA). No physical RESET needed, it's fallback only.
- Do not enable features in `config.h`/`rules.mk` to "fix" a build without checking the 28672-byte size limit.

## Decision Gates

| Situation | Action |
|-----------|--------|
| First time flashing (bootstrap) | Use physical RESET; qmk flash with -bl avrdude-split-left/right; or fallback manual avrdude + eeprom handedness write |
| After bootstrap (permanent) | Use _MEDIA-hold + QK_BOOT (Backspace hold + Q-key); physical RESET only if QK_BOOT fails |
| Firmware over size limit | Read references/gotchas.md "Firmware size limit" |
| avrdude died mid-write | Board is recoverable — press RESET (or enter bootloader via QK_BOOT), re-run script |
| `qmk doctor` fails | Read references/gotchas.md "Toolchain" |
| Both halves at once | Both need own USB cable; enter bootloader on each (bootstrap: RESET; permanent: QK_BOOT) |

## Execution Steps

### Bootstrap (one-time per half, before QK_BOOT works)

1. Compile: `qmk compile -kb crkbd/rev1 -km pones`. Confirm size warning stays under 28672 bytes.
2. Run `.claude/skills/flash-corne/assets/flash-macos.sh bootstrap [halves]` (default 2).
3. Tell the user to press RESET on one half; script flashes it with handedness mark via `qmk flash -bl avrdude-split-left` (or `-right`), then repeats for next half.
4. If pty race hits, use fallback: manual `avrdude` command + eeprom handedness write (see gotchas.md).
5. Verify: `ioreg -p IOUSB -w0 | rg '\+\-o' | rg -i corne` shows each half re-enumerated as "Corne".

### Permanent (after bootstrap, all future flashes)

1. Compile: `qmk compile -kb crkbd/rev1 -km pones`. Confirm size warning stays under 28672 bytes.
2. Plug USB into the half you want to flash first.
3. Enter bootloader: hold Backspace, tap the Q-key position (row 1, column 1 of _MEDIA layer) = QK_BOOT enters the half's bootloader.
4. Run `.claude/skills/flash-corne/assets/flash-macos.sh` (default 2 halves).
5. Script will detect port; for second half, unplug USB, plug into other half, press QK_BOOT again, script detects and flashes.
6. Verify: `ioreg -p IOUSB -w0 | rg '\+\-o' | rg -i corne` shows both halves re-enumerated as "Corne".

## Output Contract

Report: compile size (bytes/limit), each half's flash result (verified bytes), and re-enumeration check. On failure, quote the avrdude error and the matching gotcha.

## References

- `assets/flash-macos.sh` — proven watcher + avrdude flash script.
- `references/gotchas.md` — failure modes: pty race, wrong ports, size limit, recovery, toolchain setup.
