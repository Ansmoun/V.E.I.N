#ifndef INPUT_H
#define INPUT_H

#include "raylib.h"
#include <stdbool.h>

// Enumeración de las acciones físicas del vehículo
typedef enum {
    INPUT_ACCELERATE,
    INPUT_BRAKE,
    INPUT_LEFT,
    INPUT_RIGHT,
    INPUT_HANDBRAKE,
    INPUT_ACTION_COUNT
} GameAction;

// Declaración adelantada: Avisa al compilador de la existencia del tipo
// sin forzar la inclusión de car.h en módulos que no lo necesitan (como options.c)
struct Car;

// --- SISTEMA DE MAPEADO DINÁMICO ---
void InitInputSystem(void);
int GetActionKey(GameAction action);
void SetActionKey(GameAction action, int key);
bool IsActionDown(GameAction action);
bool IsActionPressed(GameAction action);

// --- NAVEGACIÓN DE MENÚS Y PANELES ---
bool IsMenuUpPressed(void);
bool IsMenuDownPressed(void);
bool IsMenuLeftPressed(void);
bool IsMenuRightPressed(void);
bool IsMenuConfirmPressed(void);

// --- CONTROL DE FLUJO GLOBAL Y TELEMETRÍA ---
void UpdateSystemInputs(bool *devMode);
void UpdateDeveloperShortcuts(bool *devMode, struct Car *playerCar);

bool IsMenuGaragePressed(void);
bool IsGameplayExitPressed(void);
bool IsTriggerDialoguePressed(void);
bool IsDialogueAdvancePressed(void);
bool IsGameplayHandbrakePressed(void);
// --- UTILIDADES DE INTERFAZ ---
const char* GetKeyNameStr(int key);

#endif // INPUT_H
