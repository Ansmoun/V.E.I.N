#include "hud.h"
#include "raylib.h"
#include "input.h"
#include <math.h>
#include <stddef.h>

void DrawCarHUD(const Car *car) {
    if (car == NULL) return;

    // 1. Convertir la velocidad interna a Km/h ficticios (Escala Arcade)
    int speedKmh = (int)(fabsf(car->speed) * 3.6f);

    // 2. Definir coordenadas base en la esquina inferior derecha de la pantalla
    int hudX = GetScreenWidth() - 230;
    int hudY = GetScreenHeight() - 150;

    // Dibujar caja de fondo traslúcida para el panel de telemetría
    DrawRectangle(hudX, hudY, 210, 130, Fade(BLACK, 0.65f));
    DrawRectangleLines(hudX, hudY, 210, 130, DARKGRAY);

    // 3. Renderizar indicador de Marcha / Relación de Transmisión (Gear)
    char gearStr[2] = "N";
    if (car->speed < -0.1f) {
        gearStr[0] = 'R'; // Marcha Atrás
    } else if (car->currentGear == 0 && fabsf(car->speed) < 0.1f) {
        gearStr[0] = 'N'; // Neutro / Ralentí
    } else {
        gearStr[0] = '1' + car->currentGear; 
    }
    
    // Sombra trasera y texto principal de la marcha
    DrawText(gearStr, hudX + 22, hudY + 22, 52, BLACK);
    DrawText(gearStr, hudX + 20, hudY + 20, 52, ORANGE);

    // 4. Mostrar Velocidad Numérica Digital
    DrawText(TextFormat("%03d", speedKmh), hudX + 85, hudY + 25, 38, RAYWHITE);
    DrawText("km/h", hudX + 160, hudY + 45, 12, LIGHTGRAY);

    // 5. Barra del Tacómetro Lineal (Estilo cuadro digital noventero)
    int barWidth = 170;
    int barHeight = 10;
    int barX = hudX + 20;
    int barY = hudY + 85;

    // Fondo gris de la barra de RPM
    DrawRectangle(barX, barY, barWidth, barHeight, GRAY);

    // Calcular factor de llenado mapeando las RPM actuales
    float rpmRatio = car->rpm / car->maxRpm;
    if (rpmRatio > 1.0f) rpmRatio = 1.0f;
    if (rpmRatio < 0.0f) rpmRatio = 0.0f;
    int filledWidth = (int)(barWidth * rpmRatio);

    // Color dinámico según el rango de revoluciones (Verde -> Amarillo -> Línea Roja)
    Color rpmColor = GREEN;
    if (rpmRatio > 0.85f) {
        rpmColor = RED;       // Zona de corte / Redline
    } else if (rpmRatio > 0.65f) {
        rpmColor = GOLD;      // Rango óptimo de par / VTEC / Powerband
    }

    // Dibujar barra de progreso y contorno analógico
    DrawRectangle(barX, barY, filledWidth, barHeight, rpmColor);
    DrawRectangleLines(barX, barY, barWidth, barHeight, LIGHTGRAY);
    
    // Telemetría numérica fina de las RPM
    DrawText(TextFormat("RPM: %d", (int)car->rpm), barX, barY + 16, 11, LIGHTGRAY);

    // 6. Alerta visual de freno de mano activo
    if (IsGameplayHandbrakePressed()) {
        if (((int)(GetTime() * 5) % 2) == 0) {
            DrawRectangle(hudX, hudY - 28, 210, 22, RED);
            DrawText("⚠️ FRENO DE MANO / DRIFT", hudX + 12, hudY - 24, 12, RAYWHITE);
        }
    }
}
