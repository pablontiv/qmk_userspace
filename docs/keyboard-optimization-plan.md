# QMK Keyboard Optimization Plan

## Overview

Este documento presenta un plan de mejoras para el keymap del teclado Corne (crkbd) basado en las mejores prácticas de QMK 2024 y técnicas de optimización modernas.

### Estado Actual
- **Keymap base**: Funcional con home row mods, tap dances, y combos
- **Características implementadas**: OLED display, RGB Matrix, tap dances optimizados
- **Organización**: Código modularizado en archivos separados (tap_dance.c, macros.c, oled.c)

### Objetivos del Plan
- Migrar a configuración data-driven (keyboard.json)
- Mejorar organización del código con estructura modular
- Implementar características modernas de QMK
- Optimizar rendimiento y responsividad
- Reducir tamaño del firmware

---

## Phase 1: Data-Driven Configuration Migration

### Objetivos
- Modernizar configuración usando keyboard.json
- Simplificar mantenimiento
- Aprovechar herramientas automáticas de QMK

### 1.1 Create keyboard.json
**Location**: `keyboards/crkbd/rev1/keymaps/pones/keyboard.json`

**Configuration to migrate from config.h**:
```json
{
    "keyboard_name": "Corne Rev1 Pones",
    "usb": {
        "vid": "0x4653",
        "pid": "0x0001",
        "device_version": "1.0.0",
        "polling_interval": 1
    },
    "features": {
        "oled": true,
        "rgb_matrix": true,
        "combo": true,
        "tap_dance": true,
        "layer_lock": true,
        "caps_word": true
    },
    "oled": {
        "timeout": 30000,
        "brightness": 255,
        "rotation": 270
    },
    "rgb_matrix": {
        "animations": {
            "rainbow_moving_chevron": true,
            "digital_rain": true,
            "multisplash": true
        },
        "led_process_limit": 26,
        "led_flush_limit": 16,
        "hue_steps": 8,
        "sat_steps": 8,
        "val_steps": 8,
        "speed_steps": 10
    },
    "split": {
        "enabled": true,
        "master": "left",
        "transport": "serial"
    },
    "tapping": {
        "term": 220,
        "per_key": true
    },
    "combo": {
        "term": 30
    },
    "debounce": 5,
    "debounce_type": "asym_eager_defer_pk"
}
```

### 1.2 Update config.h
**Keep only**:
- Custom behavior definitions
- User-specific macros
- Per-key tapping term function
- Custom feature implementations

**Remove**:
- Hardware definitions (move to keyboard.json)
- Standard feature flags (move to keyboard.json)
- OLED and RGB configuration (move to keyboard.json)

### 1.3 Update rules.mk
**Simplify to**:
```makefile
# Feature source files
SRC += tap_dance.c
SRC += macros.c

ifeq ($(strip $(OLED_ENABLE)), yes)
    SRC += oled.c
endif

# All other features now in keyboard.json
```

---

## Phase 2: Code Modularization

### Objetivos
- Mejorar organización del código
- Facilitar mantenimiento
- Permitir reutilización de características

### 2.1 Create features/ Directory Structure
```
keyboards/crkbd/rev1/keymaps/pones/
├── features/
│   ├── combos.c          # Extract from keymap.c
│   ├── combos.h          # Combo declarations
│   ├── tap_dance.c       # Already exists
│   ├── tap_dance.h       # Already exists
│   ├── macros.c          # Already exists
│   ├── macros.h          # Custom keycode declarations
│   ├── oled.c            # Already exists
│   └── oled.h            # OLED function declarations
├── keymap.c              # Main keymap definition only
├── keyboard.json         # Data-driven configuration
├── config.h              # Minimal user preferences
└── rules.mk              # Build configuration
```

### 2.2 Extract Combos Module
**Current state**: Combo definitions are in keymap.c (lines 102-115)
**Target**: Move to `features/combos.c`

```c
// features/combos.h
#pragma once
#include QMK_KEYBOARD_H

extern combo_t key_combos[];
extern uint16_t COMBO_LEN;

// features/combos.c
#include "combos.h"
#include "keymap.h" // for home row mod definitions

const uint16_t PROGMEM combo_esc[] = {HM_J, HM_K, COMBO_END};
const uint16_t PROGMEM combo_tab[] = {HM_D, HM_F, COMBO_END};
// ... rest of combos

combo_t key_combos[] = {
    COMBO(combo_esc, KC_ESC),
    COMBO(combo_tab, KC_TAB),
    // ... rest of combo array
};
uint16_t COMBO_LEN = ARRAY_SIZE(key_combos);
```

### 2.3 Update Build System
```makefile
# rules.mk
SRC += features/tap_dance.c
SRC += features/macros.c
SRC += features/combos.c

ifeq ($(strip $(OLED_ENABLE)), yes)
    SRC += features/oled.c
endif
```

---

## Phase 3: Performance Optimization

### 3.1 Debouncing Configuration
**Current**: Default symmetric defer (5ms)
**Proposed**: Asymmetric eager defer per-key

**Benefits**:
- Faster key registration on press
- Per-key debouncing for better responsiveness
- Maintains stability on release

### 3.2 USB Polling Rate
**Implementation**: Set polling interval to 1ms (1000Hz)
```json
// In keyboard.json
"usb": {
    "polling_interval": 1
}
```

**Expected results**:
- Reduced input latency
- Better gaming performance
- Improved responsiveness

### 3.3 Firmware Size Optimization
**Current optimizations (keep)**:
- `LTO_ENABLE = yes`
- Disabled unused features
- Selective RGB animations

**Additional optimizations**:
- Use PROGMEM for constant data
- Remove deprecated code patterns
- Consolidate duplicate strings

---

## Phase 4: Modern QMK Features

### 4.1 Layer Lock Feature
**Implementation**:
```json
// In keyboard.json
"features": {
    "layer_lock": true
}
```

**Integration**:
- Add `QK_LAYER_LOCK` keycode to navigation layer
- Replace momentary layer switches where appropriate
- Use for sticky symbol layer access

### 4.2 Caps Word Feature
**Implementation**:
```json
// In keyboard.json
"features": {
    "caps_word": true
}
```

**Configuration**:
```c
// In config.h
#define CAPS_WORD_IDLE_TIMEOUT 3000  // 3 seconds
#define CAPS_WORD_INVERT_ON_SHIFT
```

**Activation options**:
- Double-tap shift combo
- Dedicated key on navigation layer
- Replace existing Caps Lock functionality

### 4.3 RGB Keycode Migration
**Current (deprecated)**: `RGB_TOG`, `RGB_MOD`, `RGB_HUI`, etc.
**New format**: `UG_TOGG`, `UG_NEXT`, `UG_HUEU`, etc.

**Update required in**:
- Navigation layer RGB controls
- Any RGB-related macros

### 4.4 Key Override Feature (Optional)
**Use cases**:
- Shift + Backspace = Delete
- Custom modifier combinations
- Context-sensitive key behavior

---

## Phase 5: Testing & Validation

### 5.1 Compilation Tests
- [ ] Verify firmware compiles after each phase
- [ ] Check firmware size remains under limit (target: <85%)
- [ ] Ensure all features are included
- [ ] Test on actual hardware

### 5.2 Functional Tests
- [ ] Test all layers accessible
- [ ] Verify tap dance sequences work
- [ ] Check combo activations
- [ ] Validate home row mods behavior
- [ ] Test new features (Layer Lock, Caps Word)

### 5.3 Performance Tests
- [ ] Measure input latency improvement
- [ ] Check for missed keystrokes
- [ ] Verify modifier accuracy
- [ ] Test USB polling rate effectiveness

---

## Implementation Timeline

### Week 1: Data-Driven Migration
- Create keyboard.json
- Migrate configuration from config.h
- Update build system
- Test compilation and basic functionality

### Week 2: Code Modularization
- Create features/ directory structure
- Extract combos to separate module
- Update includes and build configuration
- Verify all features work correctly

### Week 3: Performance & Modern Features
- Implement debouncing optimization
- Add USB polling configuration
- Enable Layer Lock and Caps Word
- Update RGB keycodes

### Week 4: Testing & Documentation
- Comprehensive functionality testing
- Performance validation
- Update documentation
- Create backup of stable configuration

---

## Success Metrics

1. **Firmware Size**: Maintain <85% of maximum capacity
2. **Input Latency**: Achieve <5ms average latency
3. **Compilation**: Zero warnings, successful build
4. **Feature Completeness**: All planned features functional
5. **Code Quality**: Clean, documented, maintainable structure

---

## Rollback Plan

**Risk mitigation**:
- Git commit after each phase
- Backup of current working firmware (.hex files)
- Incremental testing approach
- Version tags for stable releases

**If issues arise**:
1. Revert to previous git commit
2. Flash backup firmware
3. Document issues for future reference
4. Adjust implementation approach

---

## Resources

### QMK Documentation
- [Data Driven Configuration](https://docs.qmk.fm/data_driven_config)
- [QMK Breaking Changes 2024](https://docs.qmk.fm/ChangeLog/20241124)
- [External Userspace](https://docs.qmk.fm/newbs_external_userspace)
- [Layer Lock Feature](https://docs.qmk.fm/features/layer_lock)

### Tools
- QMK CLI for compilation and configuration
- QMK Toolbox for firmware flashing
- Via/Vial for testing (if needed)
- Git for version control

### Best Practices
- Always test on actual hardware before committing changes
- Keep detailed notes of what works and what doesn't
- Document any deviations from the plan
- Regular backups of working configurations

---

*Document created: September 2025*  
*Status: Ready for implementation*  
*Next review: After Phase 1 completion*