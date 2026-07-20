# Repository Guidelines

## Project Structure & Module Organization

This is an external QMK userspace. Active firmware lives under `keyboards/<keyboard>/keymaps/<keymap>/`; the primary target is `keyboards/crkbd/rev1/keymaps/pones/`. Keep reusable Corne behavior in its `features/` directory and keymap wiring in `keymap.c`. Older and alternate keymaps (`pones-old`, `vial`, and YMD40 variants) are references, not the default build. `qmk.json` declares CI build targets, `docs/` contains layout diagrams and design notes, and `parse_keymap.py` plus `gen-keymap-svg.sh` regenerate keymap documentation.

## Build, Test, and Development Commands

Install QMK first, then configure this checkout as the overlay:

```bash
qmk setup
qmk config user.overlay_dir="$(realpath .)"
qmk compile -kb crkbd/rev1 -km pones
qmk userspace-compile
```

The specific compile validates the main Corne firmware; `qmk userspace-compile` builds every target in `qmk.json`. `make crkbd/rev1:pones` is an equivalent wrapper when `user.qmk_home` is configured. Run `./gen-keymap-svg.sh` after changing layer layouts; it requires Python and `keymap-drawer`'s `keymap` CLI.

## Coding Style & Naming Conventions

Follow `.editorconfig`: four spaces, UTF-8, final newline, and trimmed trailing whitespace; Makefiles use tabs. Format C with the repository `.clang-format` (Google-based, four-space indentation). Preserve QMK naming conventions: uppercase keycodes and constants (`KC_ESC`, `HM_A`), uppercase layer enum values (`_NAV`), and snake_case functions or data. Keep feature declarations in matching `.h`/`.c` pairs.

## Testing Guidelines

There is no standalone unit-test suite. Treat a clean firmware compile as the minimum test for every C, header, `config.h`, or `rules.mk` change. Run `qmk userspace-compile` before opening a PR and report the tested target and firmware-size result. Pushes also trigger the GitHub Actions build and publish workflow.

## Commit & Pull Request Guidelines

History uses Conventional Commits with optional scopes, for example `feat(keymap): add navigation combo`, `fix(keymap): reduce firmware size`, and `docs: update layout`. Keep commits focused. PRs should explain user-visible key behavior, list affected boards/layers, record build commands and results, and link relevant design notes or issues. Include updated SVGs when layout documentation changes.

## Security & Configuration Tips

Do not commit credentials, machine-specific paths, `.claude/settings.local.json`, CodeGraph state, or generated firmware (`*.hex`, `*.bin`, `*.uf2`). Flashing can overwrite EEPROM and handedness; follow the repository's dedicated Corne flashing instructions rather than improvising device paths.
