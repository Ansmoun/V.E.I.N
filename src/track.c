#include "track.h"

typedef struct {
    Vector3 position;
    Vector3 size;
    Color color;
} TrackElement;

#define TRACK_ELEMENT_COUNT 14
static TrackElement testTrack[TRACK_ELEMENT_COUNT];

// 1. Rejilla técnica para flujo óptico y referencias métricas
static void DrawDarkAsphaltGrid(void) {
    float startX = -40.0f;
    float endX = 80.0f;
    float startZ = -70.0f;
    float endZ = 180.0f;
    float step = 10.0f;

    for (float z = startZ; z <= endZ; z += step) {
        DrawLine3D((Vector3){ startX, 0.01f, z }, (Vector3){ endX, 0.01f, z }, DARKGRAY);
    }

    for (float x = startX; x <= endX; x += step) {
        DrawLine3D((Vector3){ x, 0.01f, startZ }, (Vector3){ x, 0.01f, endZ }, DARKGRAY);
    }
}

// 2. Inicialización del trazado en el segmento BSS (Cero malloc)
void InitTestTrack(void) {
    // RECTA PRINCIPAL (Z: 0 a 120)
    testTrack[0] = (TrackElement){ { -12.0f, 1.0f, 60.0f }, { 1.0f, 2.0f, 120.0f }, GRAY };
    testTrack[1] = (TrackElement){ {  12.0f, 1.0f, 60.0f }, { 1.0f, 2.0f, 120.0f }, GRAY };
    
    // HORQUILLA DE DRIFT (Z: 120 a 160)
    testTrack[2] = (TrackElement){ {   0.0f, 1.0f, 120.0f }, { 24.0f, 2.0f, 1.0f }, MAROON };
    testTrack[3] = (TrackElement){ {  20.0f, 1.0f, 140.0f }, { 1.0f, 2.0f, 40.0f }, GRAY };
    testTrack[4] = (TrackElement){ {  40.0f, 1.0f, 160.0f }, { 40.0f, 2.0f, 1.0f }, MAROON };
    
    // ZONA DE RETORNO PARALELA
    testTrack[5] = (TrackElement){ {  60.0f, 1.0f, 90.0f }, { 1.0f, 2.0f, 140.0f }, GRAY };
    testTrack[6] = (TrackElement){ {  40.0f, 1.0f, 20.0f }, { 1.0f, 2.0f, 60.0f }, GRAY };

    // CHICANE RÁPIDA (Obstáculos de transferencia de peso)
    testTrack[7] = (TrackElement){ {  25.0f, 1.0f, -10.0f }, { 12.0f, 2.0f, 4.0f }, DARKGREEN };
    testTrack[8] = (TrackElement){ {  45.0f, 1.0f, -30.0f }, { 12.0f, 2.0f, 4.0f }, DARKGREEN };
    
    // CIERRE Y ENLACE
    testTrack[9] = (TrackElement){ {  35.0f, 1.0f, -50.0f }, { 50.0f, 2.0f, 1.0f }, GRAY };
    testTrack[10] = (TrackElement){ {   0.0f, 1.0f, -25.0f }, { 1.0f, 2.0f, 50.0f }, GRAY };

    // POSTES DE REFERENCIA ÓPTICA (Hitos kilométricos)
    testTrack[11] = (TrackElement){ { -10.5f, 3.0f, 20.0f }, { 0.5f, 6.0f, 0.5f }, ORANGE };
    testTrack[12] = (TrackElement){ { -10.5f, 3.0f, 50.0f }, { 0.5f, 6.0f, 0.5f }, ORANGE };
    testTrack[13] = (TrackElement){ { -10.5f, 3.0f, 80.0f }, { 0.5f, 6.0f, 0.5f }, ORANGE };
}

// 3. Renderizado secuencial low-poly
void DrawTestTrack(void) {
    DrawDarkAsphaltGrid();

    for (int i = 0; i < TRACK_ELEMENT_COUNT; i++) {
        DrawCube(testTrack[i].position, testTrack[i].size.x, testTrack[i].size.y, testTrack[i].size.z, testTrack[i].color);
        DrawCubeWires(testTrack[i].position, testTrack[i].size.x, testTrack[i].size.y, testTrack[i].size.z, BLACK);
    }
}
