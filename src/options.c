#include "options.h"
#include "input.h"
#include "raylib.h"

typedef struct {
    int width;
    int height;
    const char *name;
} Resolution;

static Resolution resList[] = {
    { 640,  480,  "640x480" },
    { 800,  600,  "800x600" },
    { 1024, 768,  "1024x768" }
};
static int resCount = 3;
static int selectedResIdx = 1;

// Opciones del menú expandidas
typedef enum {
    OPT_RESOLUTION,
    OPT_FULLSCREEN,
    OPT_VSYNC,
    OPT_MAP_ACCEL,
    OPT_MAP_BRAKE,
    OPT_MAP_LEFT,
    OPT_MAP_RIGHT,
    OPT_MAP_HANDBRAKE,
    OPT_BACK,
    OPT_TOTAL_COUNT
} OptionLine;

static int currentLine = OPT_RESOLUTION;
static bool isFullscreen = false;
static bool isVsync = true;
static bool isWaitingForKey = false; // Bloquea el menú mientras escucha la nueva tecla

void UpdateOptionsMenu(GameState *state) {
    // SUB-ESTADO: Captura de nueva tecla física
    if (isWaitingForKey) {
        int pressedKey = GetKeyPressed();
        if (pressedKey != 0) {
            // Evitamos que elijan ENTER o ESC para no romper el flujo de navegación de menús
            if (pressedKey != KEY_ENTER && pressedKey != KEY_ESCAPE) {
                GameAction targetAction = (GameAction)(currentLine - OPT_MAP_ACCEL);
                SetActionKey(targetAction, pressedKey);
            }
            isWaitingForKey = false; // Volver al flujo normal del menú
        }
        return; // Detiene la navegación ordinaria mientras configura
    }

    // Navegación vertical convencional
    if (IsMenuDownPressed())  currentLine = (currentLine + 1) % OPT_TOTAL_COUNT;
    if (IsMenuUpPressed())    currentLine = (currentLine - 1 + OPT_TOTAL_COUNT) % OPT_TOTAL_COUNT;

    // Gestión de opciones
    switch (currentLine) {
        case OPT_RESOLUTION:
            if (IsMenuRightPressed()) {
                selectedResIdx = (selectedResIdx + 1) % resCount;
                SetWindowSize(resList[selectedResIdx].width, resList[selectedResIdx].height);
            }
            if (IsMenuLeftPressed()) {
                selectedResIdx = (selectedResIdx - 1 + resCount) % resCount;
                SetWindowSize(resList[selectedResIdx].width, resList[selectedResIdx].height);
            }
            break;

        case OPT_FULLSCREEN:
            if (IsMenuLeftPressed() || IsMenuRightPressed() || IsMenuConfirmPressed()) {
                isFullscreen = !isFullscreen;
                ToggleFullscreen();
            }
            break;

        case OPT_VSYNC:
            if (IsMenuLeftPressed() || IsMenuRightPressed() || IsMenuConfirmPressed()) {
                isVsync = !isVsync;
                if (isVsync) SetWindowState(FLAG_VSYNC_HINT);
                else ClearWindowState(FLAG_VSYNC_HINT);
            }
            break;

        // Entrar en modo de remapeo para cualquiera de los inputs de conducción
        case OPT_MAP_ACCEL:
        case OPT_MAP_BRAKE:
        case OPT_MAP_LEFT:
        case OPT_MAP_RIGHT:
        case OPT_MAP_HANDBRAKE:
            if (IsMenuConfirmPressed()) {
                isWaitingForKey = true;
            }
            break;

        case OPT_BACK:
            if (IsMenuConfirmPressed()) {
                *state = GAME_STATE_MENU;
            }
            break;
    }
}

void DrawOptionsMenu(void) {
    DrawText("CONFIGURACION DE CONTROLES Y VIDEO", 120, 40, 24, RED);

    const char *labels[] = {
        "Resolucion:",
        "Pantalla Completa:",
        "V-Sync:",
        "Acelerar:",
        "Frenar / Reversa:",
        "Girar Izquierda:",
        "Girar Derecha:",
        "Freno de Mano:",
        "VOLVER AL MENU PRINCIPAL"
    };

    int startY = 110;
    int spacing = 42; // Ajustado para que quepan holgadamente los 9 renglones en 480p/600p

    for (int i = 0; i < OPT_TOTAL_COUNT; i++) {
        Color textColor = (i == currentLine) ? GOLD : LIGHTGRAY;
        char buffer[128];

        if (i == currentLine && isWaitingForKey) {
            textColor = RED; // Feedback visual de que está escuchando el teclado
        }

        switch (i) {
            case OPT_RESOLUTION:
                TextCopy(buffer, TextFormat("%s  < %s >", labels[i], resList[selectedResIdx].name));
                break;
            case OPT_FULLSCREEN:
                TextCopy(buffer, TextFormat("%s  < %s >", labels[i], isFullscreen ? "SI" : "NO"));
                break;
            case OPT_VSYNC:
                TextCopy(buffer, TextFormat("%s  < %s >", labels[i], isVsync ? "ACTIVO" : "DESACTIVO"));
                break;
            
            // Pintar la tecla actualmente mapeada obteniéndola desde input.c
            case OPT_MAP_ACCEL:
            case OPT_MAP_BRAKE:
            case OPT_MAP_LEFT:
            case OPT_MAP_RIGHT:
            case OPT_MAP_HANDBRAKE: {
                GameAction act = (GameAction)(i - OPT_MAP_ACCEL);
                const char* keyName = GetKeyNameStr(GetActionKey(act));
                if (i == currentLine && isWaitingForKey) {
                    TextCopy(buffer, TextFormat("%s  [ PULSA UNA TECLA... ]", labels[i]));
                } else {
                    TextCopy(buffer, TextFormat("%s  [ %s ]", labels[i], keyName));
                }
                break;
            }
            case OPT_BACK:
                TextCopy(buffer, labels[i]);
                break;
        }

        if (i == currentLine) {
            DrawText(">", 70, startY + (i * spacing), 20, GOLD);
        }
        DrawText(buffer, 100, startY + (i * spacing), 20, textColor);
    }

    DrawText("[Flechas/WASD] Navegar  |  [ENTER] Modificar Tecla / Opción", 100, 520, 14, GRAY);
}
