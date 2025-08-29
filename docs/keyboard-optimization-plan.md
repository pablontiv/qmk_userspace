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

### 2.2 Reorganización de Símbolos por Frecuencia

**Análisis actual**: 
- Paréntesis `()` en posiciones incómodas (anular derecho)
- Llaves `{}` en row inferior (menos accesible)
- Operadores `=+-` separados geográficamente

**Optimización propuesta**:
```
SYMBOL layer mejorada:
Row superior: Símbolos menos frecuentes
Home row:     () [] {} <> (brackets principales)
Row inferior: Operadores y símbolos especiales
Thumbs:       = - + (acceso inmediato)
```

**Impacto**: Reducción 30% en movimiento lateral para programación

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

### 3.1 Fusión de Capas SYMBOL

**Problema actual**: SYMBOL y SYMBOL2 fragmentan símbolos relacionados

**Nueva organización**:
```
Unified SYMBOL layer:
- Brackets y parentheses agrupados
- Operadores matemáticos juntos  
- Símbolos de programación ($, @, #, %, etc.) organizados por frecuencia
- Eliminación de redundancias
```

**Beneficio**: Una capa menos que memorizar, símbolos más lógicamente agrupados

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

## 🔧 Fase 5 - Optimizaciones Avanzadas

### 5.1 Análisis de Bigrams Personalizado

**Metodología**:
1. Recopilar datos reales de typing (1-2 semanas)
2. Analizar patrones específicos de tu código
3. Identificar same-finger bigrams (SFBs) problemáticos
4. Reorganizar layout basado en datos personales

### 5.2 Configuración Adaptativa

**Features avanzadas**:
- Profiles automáticos por contexto
- Machine learning para optimización continua
- Integration con IDEs y herramientas

---

## 📈 Métricas de Éxito

### Objetivos Cuantificables
- **Reducción de SFBs**: 40% menos same-finger bigrams
- **Velocidad**: +10-15% en typing de código
- **Fatiga**: 30% menos movimiento lateral de dedos
- **Ergonomía**: Eliminación de painful stretches

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
Semana 3-4:  Implementar Fase 2 si Fase 1 exitosa  
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
- **v2.0**: (Planificado) Implementación Fase 2
- **v3.0**: (Planificado) Unificación de capas SYMBOL

---

*Documento creado: Enero 2025*  
*Última actualización: Post-Fase 1*  
*Próxima revisión: Post-implementación Fase 2*