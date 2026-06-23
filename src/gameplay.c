#include "gameplay.h"
#include "raylib.h"
#include "rlgl.h"
#include "physics.h"
#include "input.h"
#include "hud.h"
#include "track.h"

void UpdateGameplayState(Car *playerCar, Camera3D *camera, GameState *state, float dt) {
    UpdateCarDriving(playerCar, dt);      
    UpdateUniversalPhysics(playerCar, dt); 
    UpdateGameplayCamera(camera, playerCar, dt);

    if (IsGameplayExitPressed()) *state = GAME_STATE_MENU;
}

void DrawGameplayState(const Camera3D *camera, const Car *playerCar, const char *carName, bool devMode) {
    // --- 1. RENDERIZADO EN ESPACIO 3D ---
    BeginMode3D(*camera);
        DrawTestTrack();
        rlPushMatrix(); 
            rlTranslatef(playerCar->position.x, playerCar->position.y, playerCar->position.z);
            rlRotatef(playerCar->rotation, 0.0f, 1.0f, 0.0f);
            rlRotatef(playerCar->roll, 0.0f, 0.0f, 1.0f);
            rlRotatef(playerCar->pitch, 1.0f, 0.0f, 0.0f);
            
            // Chasis base del coche
            DrawCube((Vector3){ 0.0f, 0.0f, 0.0f }, 1.5f, 1.0f, 2.5f, RED);
            DrawCubeWires((Vector3){ 0.0f, 0.0f, 0.0f }, 1.5f, 1.0f, 2.5f, MAROON);
        rlPopMatrix(); 
    EndMode3D();

    // --- 2. INTERFAZ GRÁFICA 2D (HUD) ---
    DrawText(TextFormat("Auto: %s", carName), 20, 50, 18, WHITE);
    DrawText(TextFormat("Vel: %.1f km/h", playerCar->speed * 3.6f), 20, 80, 18, GREEN);
    DrawText("[ESC] Volver al Menu", 20, 110, 14, GRAY);

    // Tacómetro (Esquina inferior derecha)
    DrawCarHUD(playerCar); 

    // Panel de depuración técnica (F3)
    if (devMode) {
        int panelWidth = 280;
        int panelHeight = 210;
        int posX = GetScreenWidth() - panelWidth - 15;
        int posY = 15;
        DrawRectangle(posX, posY, panelWidth, panelHeight, Fade(BLACK, 0.75f));
        DrawRectangleLines(posX, posY, panelWidth, panelHeight, LIME);
        DrawText("--- TELEMETRIA DEV ---", posX + 15, posY + 15, 14, LIME);
        DrawText(TextFormat("Pos X:  %.3f", playerCar->position.x), posX + 15, posY + 45, 14, RAYWHITE);
        DrawText(TextFormat("Pos Y:  %.3f", playerCar->position.y), posX + 15, posY + 65, 14, RAYWHITE);
        DrawText(TextFormat("Pos Z:  %.3f", playerCar->position.z), posX + 15, posY + 85, 14, RAYWHITE);
        DrawText(TextFormat("Angulo: %.2f deg", playerCar->rotation), posX + 15, posY + 115, 14, RAYWHITE);
        DrawText(TextFormat("Roll:   %.2f deg", playerCar->roll), posX + 15, posY + 135, 14, RAYWHITE);
        DrawText(TextFormat("Pitch:  %.2f deg", playerCar->pitch), posX + 15, posY + 155, 14, RAYWHITE);
        DrawText(TextFormat("FPS:    %d", GetFPS()), posX + 15, posY + 180, 14, LIME);
    }
}
