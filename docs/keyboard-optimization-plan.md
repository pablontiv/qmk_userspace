# QMK Keyboard Optimization Plan

## Overview

Este documento presenta el plan de mejoras para el keymap del teclado Corne (crkbd) basado en las mejores prácticas de QMK 2024 y técnicas de optimización modernas.

### Estado Actual
- **Keymap base**: ✅ Funcional con home row mods, tap dances, y combos
- **Características implementadas**: ✅ OLED display, RGB Matrix, tap dances optimizados  
- **Organización**: ✅ Código modularizado en archivos separados (tap_dance.c, macros.c, oled.c)
- **Performance**: ✅ Debouncing asimétrico, USB polling 1000Hz
- **Modern Features**: ✅ Caps Word, RGB keycodes actualizados

### Fases Completadas
- ✅ **Phase 3**: Performance Optimization 
- ✅ **Phase 4 (partial)**: Caps Word + RGB Migration

---

## Phase 1: Data-Driven Configuration Migration

### Status: ⚠️ SKIPPED - No compatible con userspace

**Razón**: QMK userspace no soporta keyboard.json. La configuración debe permanecer en config.h y rules.mk.

---

## Phase 2: Code Modularization

### Status: 🔄 OPCIONAL - Estructura actual funcional

**Estado actual**: Ya tienes estructura modular básica en `features/`
- ✅ tap_dance.c, macros.c, oled.c ya están separados
- ✅ Build system configurado correctamente

**Pendiente opcional**:
- Extraer combos a `features/combos.c` para mayor organización
- Crear headers (.h) para cada módulo

---

## Phase 4: Modern QMK Features  

### 4.1 Layer Lock Feature
**Status**: 🔄 PENDIENTE

```makefile
# rules.mk - agregar
LAYER_LOCK_ENABLE = yes
```

**Integration**:
- Agregar `QK_LAYER_LOCK` a capa NAV
- Usar para acceso "sticky" a capas de símbolos

### 4.2 ✅ Caps Word Feature - COMPLETADO
- ✅ Habilitado con `CAPS_WORD_ENABLE = yes`
- ✅ Activación con doble tap en F o J
- ✅ Configurado para escribir CONSTANTS_LIKE_THIS

### 4.3 ✅ RGB Keycode Migration - COMPLETADO
- ✅ `RGB_TOG` → `UG_TOGG`  
- ✅ `RGB_MOD` → `UG_NEXT`
- ✅ `RGB_SPD/SPI` → `UG_SPDD/SPDU`

### 4.4 Key Override Feature
**Status**: ❌ INNECESARIO - Ya cubres todo con home row mods y layers

---

## Próximas mejoras opcionales

### Layer Lock Feature
**Beneficio**: Capas "sticky" sin mantener tecla presionada
**Implementación**: Agregar `LAYER_LOCK_ENABLE = yes` y `QK_LAYER_LOCK` en capa NAV

### Code Modularization 
**Beneficio**: Mayor organización del código
**Implementación**: Extraer combos a `features/combos.c`

---

## Success Metrics Alcanzados

✅ **Performance**: Debouncing asimétrico + USB polling 1000Hz  
✅ **Modern Features**: Caps Word + RGB keycodes actualizados  
✅ **Compilation**: Código compila sin errores  
✅ **Functionality**: Todas las features funcionan correctamente  

---

## Recursos Utilizados

- [Caps Word Feature](https://docs.qmk.fm/features/caps_word)
- [QMK Performance Guide](https://docs.qmk.fm/squeezing_avr)
- [QMK Breaking Changes 2024](https://docs.qmk.fm/ChangeLog/20241124)

---

*Documento actualizado: Septiembre 2025*  
*Status: Fases principales completadas*  
*Próxima revisión: Según necesidad*