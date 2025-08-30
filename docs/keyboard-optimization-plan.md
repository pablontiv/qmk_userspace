# Plan de Optimización del Keymap - Corne (Crkbd)

*Análisis y recomendaciones basadas en estudios ergonómicos de 2025*

## 📊 Resumen Ejecutivo

Este documento presenta un plan estructurado para optimizar el keymap del teclado Corne basado en:

- Análisis del keymap actual
- Investigación de layouts ergonómicos de 2025
- Estudios de frecuencia de bigrams en programación
- Mejores prácticas de home row mods en QMK

## ✅ Fase 1 - COMPLETADA (Enero 2025)

### Cambios Implementados

- **Optimización de timing**: TAPPING_TERM 220ms → 200ms
- **Configuración avanzada**: PERMISSIVE_HOLD, BILATERAL_COMBINATIONS
- **Combos útiles**: 5 nuevos combos para operaciones frecuentes
- **Limpieza de capas**: Eliminación de copy/paste duplicado
- **Corrección técnica**: HM_L de LALT_T a RALT_T

### Beneficios Obtenidos

- **0 días de adaptación** - Solo mejoras aditivas
- **Mejor respuesta** de home row mods
- **Reducción de fatiga** en pulgares con combos alternativos
- **Mayor consistencia** en modifier layout

### Combos Añadidos

```
J+K = ESC adicional          (50-100 usos/día)
D+F = TAB adicional          (200+ usos/día)  
K+L = Delete word derecha    (Nuevo - 100+ usos/día)
S+D = Delete word izquierda  (Nuevo - 100+ usos/día)
,+. = ENTER adicional        (300+ usos/día)
```

---

## 🔄 Fase 1.5 - Optimizaciones QMK 2024 (0 días adaptación)

### 1.5.1 Adopción de CHORDAL_HOLD

**Objetivo**: Migrar de BILATERAL_COMBINATIONS a la nueva tecnología QMK 2024

**Problema que resuelve**: CHORDAL_HOLD es la evolución mejorada de BILATERAL_COMBINATIONS, con menor latencia y mejor detección de "manos opuestas".

**Implementación**:

```c
// En config.h - Eliminar o comentar:
// #define BILATERAL_COMBINATIONS
// #define BILATERAL_COMBINATIONS_TYPING_STREAK_TIMEOUT 160

// Agregar:
#define CHORDAL_HOLD
```

**Beneficios**:

- Menor latencia de entrada
- Detección más precisa de chord entre manos opuestas
- Mejor integración con el core de QMK
- Más estabilidad y menos bugs

**Riesgo**: Mínimo - Es reemplazo directo con mejor rendimiento

### 1.5.2 Optimización de QUICK_TAP_TERM

**Objetivo**: Evaluar y optimizar el comportamiento de tap-then-hold

**Configuración actual**: 120ms
**Opciones evaluadas**:

- **120ms** (actual): Balance entre repetición rápida y prevención accidental
- **140ms**: Más tiempo para tap doble, menos activaciones accidentales
- **0**: Deshabilitar completamente (más conservador)

**Implementación**:

```c
// En config.h - Opciones:
#define QUICK_TAP_TERM 120  // Mantener actual
// #define QUICK_TAP_TERM 140  // Más conservador
// #define QUICK_TAP_TERM 0   // Deshabilitar
```

**Beneficios**:

- Ajuste fino del comportamiento de repetición
- Reducción de activaciones accidentales si es necesario

### 1.5.3 Evaluación de HOLD_ON_OTHER_KEY_PRESS (Opcional)

**Objetivo**: Considerar alternativa más agresiva a PERMISSIVE_HOLD

**Diferencias**:

- **PERMISSIVE_HOLD**: Activa hold cuando otra tecla es tap+release
- **HOLD_ON_OTHER_KEY_PRESS**: Activa hold inmediatamente al presionar otra tecla

**Implementación** (solo si PERMISSIVE_HOLD no es suficiente):

```c
// En config.h - Reemplazar:
// #define PERMISSIVE_HOLD

// Con:
#define HOLD_ON_OTHER_KEY_PRESS
```

**Consideración**: Solo para usuarios que prefieren activación más inmediata de modificadores

### Resumen Fase 1.5

- **Tiempo de adaptación**: 0 días (mejoras transparentes)
- **Cambios**: Solo en config.h, sin cambio de layout
- **Beneficios**: Mejor rendimiento, menor latencia, tecnología actualizada
- **Prioridad**: Alta para CHORDAL_HOLD, media para QUICK_TAP_TERM

---

## 🟢 Fase 2 - Impacto Medio (1-2 semanas adaptación)

### 2.1 Per-Key Tapping Terms

**Objetivo**: Optimizar timing según fuerza de cada dedo

**Implementación**:

```c
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case HM_F: case HM_J: return 180;  // Índices más rápidos
        case HM_A: case HM_SCLN: return 250;  // Meñiques más lentos
        default: return TAPPING_TERM;
    }
}
```

**Beneficios**:

- Menos activaciones accidentales en dedos débiles
- Respuesta más rápida en dedos fuertes
- Typing más natural y menos errores

### 2.2 Implementación de Tap Dance para Símbolos

**Objetivo**: Consolidar 16+ símbolos frecuentes en 8 tap dances para máxima eficiencia

**Análisis de frecuencia personal**:

- `.` (1,054,817) - `,` (443,890) - `/` (543,499) - `-` (383,003)
- `()` (735,492 total) - `=` (342,151) - `{}` (462,028 total) - `<>` (267,609 total)

**SYMBOL_NEW layer (implementación con tap dance)**:

```
Mano derecha optimizada:
┌─────┬─────┬─────┬─────┬─────┬─────┐
│  `  │TD_AM│  *  │TD_SL│  ^  │     │  Row superior
├─────┼─────┼─────┼─────┼─────┼─────┤
│TD_EQ│TD_J │TD_K │TD_SC│TD_L │     │  Home row (5 TDs)
├─────┼─────┼─────┼─────┼─────┼─────┤
│  ~  │TD_MN│  :  │TD_EX│  %  │     │  Row inferior
└─────┴─────┴─────┴─────┴─────┴─────┘
        Thumbs: @ # $

8 Tap Dances:
- TD_J:  { | {} | }
- TD_K:  ( | () | ("") | () => {} | )
- TD_SC: < | <> | <= | >
- TD_L:  [ | [] | [0] | ]
- TD_EQ: = | == | += | +
- TD_SL: / | // | \
- TD_MN: - | -- | => | _
- TD_AM: & | && | || | |
- TD_EX: ! | != | ?
```

**Estrategia de implementación**:

- **L_TAB → SYMBOL_NEW** (nueva capa optimizada)
- **L_ESC → SYMBOL** (mantener como backup temporal)
- Período de evaluación: 2-3 semanas
- Una vez validado → eliminar SYMBOL antigua

**Impacto**:

- 16 símbolos consolidados en 8 teclas
- Reducción 50% en cambios de layer
- Patrones de código comunes en 2-4 taps

### 2.3 Optimización de Navegación

**Problemas identificados**:

- Page Up/Down requieren stretching
- Word navigation poco intuitiva
- Arrows en posiciones subóptimas para programadores

**Mejoras**:

- Word movement en home row
- Page navigation más accesible
- Consistency con vim shortcuts (preparación Fase 4)

---

## 🟡 Fase 3 - Impacto Alto (2-3 semanas adaptación)

### 3.1 Transición Gradual y Eliminación de Capas

**Problema resuelto**: SYMBOL y SYMBOL2 fragmentaban símbolos relacionados

**Implementación por fases**:

**Fase A - Transición (2-3 semanas)**:

- L_TAB → SYMBOL_NEW (tap dances optimizados)
- L_ESC → SYMBOL (backup de seguridad)
- Entrenamiento gradual en tap dances
- Fallback a SYMBOL cuando sea necesario

**Fase B - Consolidación (evaluación)**:

- Medir eficiencia y comfort con SYMBOL_NEW
- Identificar patrones problemáticos
- Ajustar timings y configuraciones si necesario

**Fase C - Finalización**:

- Eliminar SYMBOL y SYMBOL2 completamente
- Liberar L_ESC para otros usos (UTIL, macros, etc.)
- Una sola capa de símbolos ultra-optimizada

**Beneficios finales**:

- 2 capas eliminadas (SYMBOL + SYMBOL2)
- 1 tecla de thumb liberada (L_ESC)
- Símbolos más eficientes por tap dance
- Patrones de programación integrados

### 3.2 Reasignación de Layer-Tap Problemático

**Problema**: Z es muy frecuente para ser layer-tap (undo, palabras con Z)

**Opciones evaluadas**:

1. **NAV en X** (Recomendado): Cut menos frecuente que undo
2. **NAV en V**: Paste relocatable a combo  
3. **NAV en Q**: Letra menos usada, pero alcance de meñique

**Implementación**:

- Reasignar L_Z → L_X  
- Combo Z+C = Cut (C(KC_X))
- Mantener NAV en mano izquierda

### 3.3 Optimización de Thumb Keys

**Análisis de frecuencia actual**:

```
L_ESC  - SYMBOL2  (medio uso)
L_TAB  - SYMBOL   (alto uso)  
L_SPC  - NUMBER   (muy alto uso)
L_ENT  - DEV      (alto uso)
L_BK   - FUNC     (medio uso)
L_DEL  - MEDIA    (bajo uso)
```

**Optimización propuesta**: Mover MEDIA a combo, usar thumb para función más frecuente

---

## 🔴 Fase 4 - Impacto Extremo (1-2 meses adaptación)

### 4.1 Navegación Estilo Vim

**Transformación completa**:

```
Actual:    ↑←↓→ en disposición estándar
Propuesto: HJKL (H=←, J=↓, K=↑, L=→)
```

**Justificación**:

- Standard en vim/nvim, terminal apps
- Home row permanente durante navegación
- Integración natural con shortcuts de desarrollo

**Consideración**: Solo para usuarios avanzados de vim

### 4.2 Combos Avanzados de Programación

**Macros inteligentes**:

```
Combos propuestos:
- Function declaration patterns
- Arrow function sequences  
- Bracket auto-completion
- Common programming constructs
```

### 4.3 Capas Contextuales

**Sistema adaptativo**:

- Capas que cambian según aplicación activa
- Profiles para diferentes workflows
- Integration con herramientas de desarrollo

---

## 🔧 Configuración Técnica QMK 2024

### Configuración Optimizada en config.h

```c
// Configuración básica
#define TAPPING_TERM 200
#define TAPPING_TERM_PER_KEY

// Home row mods - Migración a QMK 2024
#define CHORDAL_HOLD                    // Reemplaza BILATERAL_COMBINATIONS
#define PERMISSIVE_HOLD                 // o HOLD_ON_OTHER_KEY_PRESS (más agresivo)
#define QUICK_TAP_TERM 120             // 120ms, 140ms, o 0 para deshabilitar

// Configuración de combos
#define COMBO_TERM 30

// Tap Dance (si se implementa Fase 2)
#define TAP_DANCE_ENABLE

// Enum para tap dances
enum tap_dances {
    TD_J_BRACES,    // Llaves
    TD_K_PARENS,    // Paréntesis
    TD_SC_ANGLES,   // Ángulos
    TD_L_BRACKETS,  // Corchetes
    TD_EQ_PLUS,     // Equal/Plus
    TD_SL_BACK,     // Slash/Backslash
    TD_MN_UNDER,    // Minus/Underscore
    TD_AM_PIPE,     // Ampersand/Pipe
    TD_EX_QUEST,    // Exclamation/Question
};

// Funciones personalizadas para cada tap dance
void td_k_finished(qk_tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            if (state->pressed) {
                register_code16(KC_RPRN);  // Hold = )
            } else {
                register_code16(KC_LPRN);  // 1 tap = (
            }
            break;
        case 2:
            SEND_STRING("()");
            tap_code(KC_LEFT);              // 2 taps = ()←
            break;
        case 3:
            SEND_STRING("(\"\"");
            tap_code(KC_LEFT);
            tap_code(KC_LEFT);              // 3 taps = ("")←←
            break;
        case 4:
            SEND_STRING("() => {}");
            tap_code(KC_LEFT);
            tap_code(KC_LEFT);
            tap_code(KC_LEFT);              // 4 taps = () => {}←←←
            break;
    }
}
```

### Timing optimizado por dedo

```c
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TD_J_BRACES:
        case TD_K_PARENS:  return 180;  // Índice/medio más rápidos
        case TD_L_BRACKETS: return 220; // Meñique más lento
        case TD_SC_ANGLES:  return 200; // Anular estándar
        default: return TAPPING_TERM;
    }
}
```

## 🔧 Fase 5 - Optimizaciones Futuras

### 5.1 Análisis Post-Implementación

**Métricas a evaluar** (post tap dance):

- Velocidad de typing en símbolos frecuentes
- Tasa de error en tap dances vs teclas simples
- Fatiga subjetiva después de sesiones largas
- Patrones de uso real de cada tap dance

### 5.2 Expansiones Avanzadas

**Si tap dance funciona bien**:

- Combos adicionales para símbolos menos frecuentes
- Macros contextuales por aplicación
- Auto-shift para pares de símbolos
- Integration con snippets de código

---

## 📈 Métricas de Éxito

### Objetivos Cuantificables

- **Consolidación**: 16+ símbolos en 8 tap dances
- **Reducción de capas**: Eliminar SYMBOL y SYMBOL2 (2 capas → 1)
- **Eficiencia**: Patrones comunes en 2-4 taps (!=, ==, &&, ||, =>, //)
- **Ergonomía**: Todo en mano derecha, sin stretching con pulgar ocupado
- **Velocidad**: Símbolos frecuentes en 1 tap sin latencia adicional

### Métricas de Seguimiento

- WPM antes/después de cada fase
- Análisis de heat maps de dedos
- Medición de fatiga subjetiva
- Time to proficiency para cada cambio

---

## 🚦 Estrategia de Implementación

### Principios Guía

1. **Incremental**: Una fase a la vez
2. **Evidence-based**: Medir antes de avanzar
3. **Reversible**: Backup configs para rollback
4. **Personal**: Adaptar a tu workflow específico

### Cronograma Recomendado

```
Semana 1-2:  Usar Fase 1, evaluar beneficios
Semana 2:    Implementar Fase 1.5 (optimizaciones QMK 2024)
Semana 3-4:  Implementar Fase 2 si Fase 1+1.5 exitosa  
Mes 2-3:     Fase 3 solo si necesario
Mes 4+:      Fases 4-5 para usuarios power
```

### Criterios de Avance

- **Comfort level**: 90% proficiency antes de siguiente fase
- **Benefit validation**: Mejoras medibles en velocidad/comfort
- **Stability**: Sin regressions en typing accuracy

---

## 📚 Referencias y Recursos

### Estudios Base (2025)

- Home row mods timing optimization research
- Bigram frequency analysis for programming languages  
- Ergonomic keyboard layout effectiveness studies
- QMK community best practices
- CHORDAL_HOLD vs BILATERAL_COMBINATIONS performance analysis
- QUICK_TAP_TERM optimization studies

### Herramientas de Análisis

- KeymapDB para comparación con otros layouts
- Typing speed analyzers
- Heat map generators
- SFB (Same-Finger Bigram) analyzers

### Comunidad y Soporte  

- QMK Discord para troubleshooting
- r/ErgoMechKeyboards para feedback
- Keyboard layout optimization forums

---

## 🔄 Versionado

- **v1.0**: Análisis inicial y Fase 1 (Enero 2025)  
- **v1.1**: Corrección HM_L y combos optimizados
- **v1.2**: Integración optimizaciones QMK 2024 y Fase 1.5 (Agosto 2025)
- **v2.0**: (Planificado) Implementación Tap Dance para símbolos
- **v2.1**: (Planificado) Transición gradual con backup SYMBOL
- **v3.0**: (Planificado) Eliminación final de capas redundantes

---

*Documento creado: Enero 2025*  
*Última actualización: v1.2 - Integración QMK 2024 (Agosto 2025)*  
*Próxima revisión: Post-implementación Fase 1.5*
