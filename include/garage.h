#ifndef GARAGE_H
#define GARAGE_H

#include "common.h"
#include "car.h"

// Inicializa el garaje escaneando el directorio de coches
void InitGarage(void);
void CloseGarage(void);

// Getters públicos para que otros módulos consulten el auto activo
const char* GetSelectedCarFile(void);
const char* GetSelectedCarName(void);

// Controladores de estado (Lógica y Renderizado)
void UpdateGarageState(GameState *state, Car *playerCar);
void DrawGarageState(void);

#endif
