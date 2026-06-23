#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "common.h"
#include "car.h"
#include "camera.h"

void UpdateGameplayState(Car *playerCar, Camera3D *camera, GameState *state, float dt);
void DrawGameplayState(const Camera3D *camera, const Car *playerCar, const char *carName, bool devMode);

#endif
