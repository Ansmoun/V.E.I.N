#include "menu.h"
#include "raylib.h"
#include "input.h"
#include "garage.h"

static int currentMainLine = MAIN_MENU_STORY;
static float logoTimer = 0.0f;

void UpdateLogoState(GameState *state, float dt) {
    logoTimer += dt;
    if (logoTimer >= 3.0f || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        *state = GAME_STATE_MENU;
    }
}

void DrawLogoState(void) {
    DrawText("V.E.I.N. INTRO", 280, 260, 32, RAYWHITE);
}

void UpdateMenuState(GameState *state, Car *playerCar) {
    if (IsMenuDownPressed()) currentMainLine = (currentMainLine + 1) % MAIN_MENU_COUNT;
    if (IsMenuUpPressed())   currentMainLine = (currentMainLine - 1 + MAIN_MENU_COUNT) % MAIN_MENU_COUNT;
    if (IsMenuConfirmPressed()) {
        switch (currentMainLine) {
            case MAIN_MENU_STORY:
                // Obtiene la ruta de forma dinámica desde el sistema del garaje
                InitCarFromJSON(playerCar, GetSelectedCarFile());
                *state = GAME_STATE_DIALOGUE; 
                break;
            case MAIN_MENU_GARAGE:
                *state = GAME_STATE_GARAGE;
                break;
            case MAIN_MENU_OPTIONS:
                *state = GAME_STATE_OPTIONS;
                break;
        }
    }
}

void DrawMenuState(void) {
    DrawText("V.E.I.N. SPEEDCORE", 180, 120, 44, RED);
    const char *mainLabels[] = {
        "MODO HISTORIA (Dialogos de prueba)",
        "ENTRAR AL GARAJE",
        "CONFIGURACION / OPCIONES"
    };
    int startY = 240;
    int spacing = 48;
    for (int i = 0; i < MAIN_MENU_COUNT; i++) {
        Color textColor = (i == currentMainLine) ? GOLD : LIGHTGRAY;
        if (i == currentMainLine) DrawText(">", 180, startY + (i * spacing), 20, GOLD);
        DrawText(mainLabels[i], 210, startY + (i * spacing), 20, textColor);
    }
    DrawText("[Flechas / WASD] Navegar  |  [ENTER / ESPACIO] Confirmar", 160, 480, 14, GRAY);
}

void UpdateDialogueState(GameState *state) {
    if (IsDialogueAdvancePressed() || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
        *state = GAME_STATE_GAMEPLAY;
    }
}

void DrawDialogueState(void) {
    DrawRectangle(50, 420, 700, 140, Fade(DARKGRAY, 0.85f));
    DrawRectangleLines(50, 420, 700, 140, RED);
    DrawText("NARRADOR:", 70, 440, 18, GOLD);
    DrawText("El motor ruge en ralentí. Presiona ESPACIO.", 70, 480, 16, WHITE);
}
