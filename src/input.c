#include "input.h"
#include "car.h"     // Necesario para expandir y mutar las propiedades físicas del coche
#include "common.h"  // Necesario para capturar la macro TARGET_FPS (85Hz)
#include <stddef.h>  // Definición nativa de NULL para el compilador

static int actionMapping[INPUT_ACTION_COUNT];

void InitInputSystem(void) {
    actionMapping[INPUT_ACCELERATE] = KEY_W;
    actionMapping[INPUT_BRAKE]      = KEY_S;
    actionMapping[INPUT_LEFT]       = KEY_A;
    actionMapping[INPUT_RIGHT]      = KEY_D;
    actionMapping[INPUT_HANDBRAKE]  = KEY_SPACE;
}

int GetActionKey(GameAction action) {
    if (action < 0 || action >= INPUT_ACTION_COUNT) return 0;
    return actionMapping[action];
}

void SetActionKey(GameAction action, int key) {
    if (action >= 0 && action < INPUT_ACTION_COUNT) {
        actionMapping[action] = key;
    }
}

bool IsActionDown(GameAction action) {
    return IsKeyDown(actionMapping[action]);
}

bool IsActionPressed(GameAction action) {
    return IsKeyPressed(actionMapping[action]);
}

// Lógica de menús (Soporta Flechas del cursor y esquema clásico WASD)
bool IsMenuUpPressed(void)      { return (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)); }
bool IsMenuDownPressed(void)    { return (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)); }
bool IsMenuLeftPressed(void)    { return (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)); }
bool IsMenuRightPressed(void)   { return (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)); }
bool IsMenuConfirmPressed(void) { return (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)); }

// Alternar telemetría básica por hardware estándar
void UpdateSystemInputs(bool *devMode) {
    if (IsKeyPressed(KEY_F3)) {
        *devMode = !(*devMode);
    }
}

// Atajos fijos de bajo nivel para desarrollo (Depuración física y rendimiento)
void UpdateDeveloperShortcuts(bool *devMode, struct Car *playerCar) {
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_LEFT_SHIFT)) {
        
        // [Ctrl + Shift + P] -> Alternar modo telemetría extendido
        if (IsKeyPressed(KEY_P)) {
            *devMode = !(*devMode);
        }
        
        // [Ctrl + Shift + 0] -> Desbloquear tasa de FPS (Benchmarking puro)
        if (IsKeyPressed(KEY_ZERO)) {
            SetTargetFPS(0); 
        }
        
        // [Ctrl + Shift + 1] -> Capar FPS al refresco objetivo de la fase (85Hz)
        if (IsKeyPressed(KEY_ONE)) {
            SetTargetFPS(TARGET_FPS);
        }

        // [Ctrl + Shift + R] -> Recuperación de emergencia (Cero absoluto)
        if (IsKeyPressed(KEY_R)) {
            if (playerCar != NULL) {
                playerCar->position = (Vector3){ 0.0f, 0.0f, 0.0f };
                playerCar->speed = 0.0f;
                playerCar->rotation = 0.0f;
                playerCar->pitch = 0.0f;
                playerCar->roll = 0.0f;
            }
        }
    }
}

bool IsMenuGaragePressed(void)      { return IsKeyPressed(KEY_G); }
bool IsGameplayExitPressed(void)    { return IsKeyPressed(KEY_ESCAPE); }
bool IsTriggerDialoguePressed(void) { return false; } // Reservado
bool IsDialogueAdvancePressed(void) { return IsKeyPressed(KEY_SPACE); }
bool IsGameplayHandbrakePressed(void) { return IsActionDown(INPUT_HANDBRAKE); }
const char* GetKeyNameStr(int key) {
    if (key >= KEY_A && key <= KEY_Z) {
        static char letter[2] = {0};
        letter[0] = (char)key;
        return letter;
    }
    switch (key) {
        case KEY_SPACE:        return "ESPACIO";
        case KEY_ENTER:        return "ENTER";
        case KEY_UP:           return "ARRIBA";
        case KEY_DOWN:         return "ABAJO";
        case KEY_LEFT:         return "IZQUIERDA";
        case KEY_RIGHT:        return "DERECHA";
        case KEY_LEFT_SHIFT:   return "L-SHIFT";
        case KEY_LEFT_CONTROL: return "L-CTRL";
        case KEY_LEFT_ALT:     return "L-ALT";
        case KEY_ZERO:         return "0";
        case KEY_ONE:          return "1";
        case KEY_TWO:          return "2";
        case KEY_THREE:        return "3";
        case KEY_FOUR:         return "4";
        case KEY_FIVE:         return "5";
        case KEY_SIX:          return "6";
        case KEY_SEVEN:        return "7";
        case KEY_EIGHT:        return "8";
        case KEY_NINE:         return "9";
        default:               return "OTRA TECLA";
    }
}
