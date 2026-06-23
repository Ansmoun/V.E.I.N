#ifndef CAMERA_H
#define CAMERA_H

#include "raylib.h"
#include "car.h"

// Actualiza la posición, objetivo y vector UP de la cámara con físicas suavizadas
void UpdateGameplayCamera(Camera3D *camera, const Car *playerCar, float dt);

#endif
