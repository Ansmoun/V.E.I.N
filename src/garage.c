#include "garage.h"
#include "raylib.h"
#include "cJSON.h"
#include "input.h"

#define MAX_GARAGE_CARS 16

typedef struct {
    char filePath[256];
    char carName[64];
} GarageCar;

// Memoria estática interna persistente durante la ejecución del juego
static GarageCar garageCars[MAX_GARAGE_CARS];
static int carCount = 0;
static int selectedCarIndex = 0;

void InitGarage(void) {
    carCount = 0;
    selectedCarIndex = 0;

    if (DirectoryExists("assets/data/cars")) {
        // Obtenemos la lista de archivos de la carpeta mediante la API de Raylib
        FilePathList files = LoadDirectoryFiles("assets/data/cars");

        for (unsigned int i = 0; i < files.count; i++) {
            // Filtrar solo archivos con extensión .json
            if (IsFileExtension(files.paths[i], ".json")) {
                if (carCount >= MAX_GARAGE_CARS) break;

                // Almacenar ruta del archivo
                TextCopy(garageCars[carCount].filePath, files.paths[i]);

                // Leer el contenido del JSON para extraer el "name" de manera interna
                char *fileText = LoadFileText(files.paths[i]);
                if (fileText != NULL) {
                    cJSON *json = cJSON_Parse(fileText);
                    if (json != NULL) {
                        cJSON *nameItem = cJSON_GetObjectItemCaseSensitive(json, "name");
                        if (nameItem != NULL && cJSON_IsString(nameItem)) {
                            TextCopy(garageCars[carCount].carName, nameItem->valuestring);
                        } else {
                            // Fallback: Si no hay clave "name", usa el nombre del archivo raw
                            TextCopy(garageCars[carCount].carName, GetFileNameWithoutExt(files.paths[i]));
                        }
                        cJSON_Delete(json);
                    }
                    UnloadFileText(fileText); // Liberamos el búfer de texto inmediatamente
                }
                carCount++;
            }
        }
        // Liberar la estructura de mapeo de directorios de Raylib
        UnloadDirectoryFiles(files);
    }

    // Mecanismo de seguridad preventiva por si la carpeta está completamente vacía
    if (carCount == 0) {
        TextCopy(garageCars[0].filePath, "assets/data/cars/integra.json");
        TextCopy(garageCars[0].carName, "Vehiculo Base de Emergencia");
        carCount = 1;
    }
}

void CloseGarage(void) {
    // Al usar arreglos estáticos en el stack/BSS no requerimos free() dinámicos
}

const char* GetSelectedCarFile(void) {
    return garageCars[selectedCarIndex].filePath;
}

const char* GetSelectedCarName(void) {
    return garageCars[selectedCarIndex].carName;
}

void UpdateGarageState(GameState *state, Car *playerCar) {
    if (IsMenuLeftPressed())  selectedCarIndex = (selectedCarIndex - 1 + carCount) % carCount;
    if (IsMenuRightPressed()) selectedCarIndex = (selectedCarIndex + 1) % carCount;
    
    if (IsMenuConfirmPressed()) {
        // Inicializamos el chasis del jugador con la ruta descubierta dinámicamente
        InitCarFromJSON(playerCar, garageCars[selectedCarIndex].filePath);
        *state = GAME_STATE_GAMEPLAY;
    }
    if (IsGameplayExitPressed()) *state = GAME_STATE_MENU;
}

void DrawGarageState(void) {
    DrawText("[ TALLER DE MODIFICACIÓN 3D ]", 210, 100, 22, BLUE);
    DrawText("SELECCIONA TU AUTO:", 210, 180, 18, LIGHTGRAY);
    
    // Muestra el nombre real extraído del JSON
    DrawText(garageCars[selectedCarIndex].carName, 210, 220, 22, YELLOW);
    
    DrawText(TextFormat("Vehiculo %d de %d", selectedCarIndex + 1, carCount), 210, 255, 14, GRAY);
    DrawText("< Usa las flechas IZQ / DER o las teclas A / D >", 210, 290, 16, GRAY);
    DrawText("Presiona ENTER o ESPACIO para confirmar y salir a pista", 210, 340, 16, GREEN);
    DrawText("Presiona ESC para regresar al menú principal", 210, 370, 14, DARKGRAY);
}
