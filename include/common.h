#ifndef COMMON_H
#define COMMON_H

// ============================================================================
// 1. CONSTANTES GLOBALES
// ============================================================================
// Usamos #define para darle un nombre fijo a un valor. 
// Si mañana quieres cambiar la resolución, solo la cambias aquí una vez.

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define TARGET_FPS    85   // Ajustado a los 85 Hz nativos de tu monitor CRT

// ============================================================================
// 2. MÁQUINA DE ESTADOS GLOBAL
// ============================================================================
// Un 'enum' es una lista de palabras que por dentro valen números (0, 1, 2...).
// Nos sirve para que el código sea fácil de leer en vez de usar números sueltos.

typedef enum GameState {
    GAME_STATE_LOGO,
    GAME_STATE_MENU,
    GAME_STATE_OPTIONS, // ¡Nuevo estado!
    GAME_STATE_GAMEPLAY,
    GAME_STATE_GARAGE,
    GAME_STATE_DIALOGUE
} GameState;


#endif // COMMON_H
