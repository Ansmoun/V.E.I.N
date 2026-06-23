#include "raylib.h"
#include "common.h"
#include "car.h"
#include "garage.h" 
#include "input.h"  
#include "options.h" 
#include "menu.h"
#include "gameplay.h"

int main(void) {
    // ------------------------------------------------------------------------
    // 1. CONFIGURACIÓN E INICIALIZACIÓN DEL ENTORNO
    // ------------------------------------------------------------------------
    SetConfigFlags(FLAG_VSYNC_HINT); 
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "V.E.I.N. Speedcore - Fase 2 (3D Base)");
    SetExitKey(KEY_NULL); 
    SetTargetFPS(TARGET_FPS);
    InitInputSystem(); 
    
    // Escaneo automático de los archivos JSON de los coches
    InitGarage(); 

    GameState currentState = GAME_STATE_LOGO;
    bool devMode = false;

    // Inicialización del chasis usando el coche por defecto descubierto
    Car playerCar;
    InitCarFromJSON(&playerCar, GetSelectedCarFile());

    // Configuración inicial de la cámara 3D
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 4.0f, -8.0f }; 
    camera.target = playerCar.position;               
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };        
    camera.fovy = 60.0f;                              
    camera.projection = CAMERA_PERSPECTIVE;           

    // ------------------------------------------------------------------------
    // 2. BUCLE PRINCIPAL DEL JUEGO
    // ------------------------------------------------------------------------
    while (!WindowShouldClose()) {
        UpdateSystemInputs(&devMode);                   
        UpdateDeveloperShortcuts(&devMode, &playerCar);
        float dt = GetFrameTime();

        // ====================================================================
        // MÁQUINA DE ESTADOS: UPDATE
        // ====================================================================
        switch (currentState) {
            case GAME_STATE_LOGO:     UpdateLogoState(&currentState, dt); break;
            case GAME_STATE_MENU:     UpdateMenuState(&currentState, &playerCar); break;
            case GAME_STATE_OPTIONS:  UpdateOptionsMenu(&currentState); break;
            case GAME_STATE_GAMEPLAY: UpdateGameplayState(&playerCar, &camera, &currentState, dt); break;
            case GAME_STATE_GARAGE:   UpdateGarageState(&currentState, &playerCar); break;
            case GAME_STATE_DIALOGUE: UpdateDialogueState(&currentState); break;
        }

        // ====================================================================
        // MÁQUINA DE ESTADOS: DRAW
        // ====================================================================
        BeginDrawing();
            ClearBackground(BLACK);

            switch (currentState) {
                case GAME_STATE_LOGO:     DrawLogoState(); break;
                case GAME_STATE_MENU:     DrawMenuState(); break;
                case GAME_STATE_OPTIONS:  DrawOptionsMenu(); break;
                case GAME_STATE_GAMEPLAY: DrawGameplayState(&camera, &playerCar, GetSelectedCarName(), devMode); break;
                case GAME_STATE_GARAGE:   DrawGarageState(); break;
                case GAME_STATE_DIALOGUE: DrawDialogueState(); break;
            }
            
            DrawFPS(10, 10);
        EndDrawing();
    } 

    // ------------------------------------------------------------------------
    // 3. LIBERACIÓN DE RECURSOS Y CIERRE
    // ------------------------------------------------------------------------
    CloseGarage();
    CloseWindow();
    return 0;
}
