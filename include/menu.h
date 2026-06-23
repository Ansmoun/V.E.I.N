#ifndef MENU_H
#define MENU_H

#include "common.h"
#include "car.h"

typedef enum {
    MAIN_MENU_STORY,
    MAIN_MENU_GARAGE,
    MAIN_MENU_OPTIONS,
    MAIN_MENU_COUNT
} MainMenuLine;

// Funciones del LOGO
void UpdateLogoState(GameState *state, float dt);
void DrawLogoState(void);

// Funciones del MENÚ PRINCIPAL
void UpdateMenuState(GameState *state, Car *playerCar);
void DrawMenuState(void);

// Funciones de DIÁLOGOS
void UpdateDialogueState(GameState *state);
void DrawDialogueState(void);

#endif
