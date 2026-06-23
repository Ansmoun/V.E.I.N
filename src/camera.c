#include "camera.h"
#include "raymath.h"
#include <math.h>

static float cameraHeading = 0.0f;

void UpdateGameplayCamera(Camera3D *camera, const Car *playerCar, float dt) {
    // 1. Cálculo del ángulo de movimiento (hacia dónde va el coche realmente)
    float gripModifier = 0.4f;
    if (playerCar->driveType == DRIVE_AWD) gripModifier = 0.70f;
    if (playerCar->driveType == DRIVE_FWD) gripModifier = 0.20f;

    float moveAngle = (playerCar->rotation * DEG2RAD) + (playerCar->visualSlipAngle * DEG2RAD * (1.0f - gripModifier));
    
    // 2. Lógica de orientación desacoplada
    float targetCamHeading = playerCar->rotation;
    if (fabsf(playerCar->visualSlipAngle) > 5.0f) {
        targetCamHeading = moveAngle * RAD2DEG;
    }

    float angleDiff = targetCamHeading - cameraHeading;
    while (angleDiff < -180.0f) angleDiff += 360.0f;
    while (angleDiff > 180.0f) angleDiff -= 360.0f;

    // Zona muerta para que la cámara no vibre en rectas
    if (fabsf(angleDiff) > 3.5f) {
        cameraHeading += angleDiff * 4.5f * dt;
    }

    // 3. Posicionamiento
    float distance = 7.0f;
    float height = 2.5f;
    float rad = cameraHeading * DEG2RAD;

    camera->target = playerCar->position;
    camera->position.x = playerCar->position.x - sinf(rad) * distance;
    camera->position.z = playerCar->position.z - cosf(rad) * distance;
    camera->position.y = playerCar->position.y + height;

    // 4. Inclinación Dinámica (Roll)
    Vector3 viewDir = { camera->target.x - camera->position.x, 0.0f, camera->target.z - camera->position.z };
    Vector3 camRight = { -viewDir.z, 0.0f, viewDir.x };
    
    float rightLen = sqrtf(camRight.x * camRight.x + camRight.z * camRight.z);
    if (rightLen > 0.001f) {
        camRight.x /= rightLen; camRight.z /= rightLen;
    }

    camera->up.x = camRight.x * (playerCar->roll * 0.035f);
    camera->up.y = 1.0f;
    camera->up.z = camRight.z * (playerCar->roll * 0.035f);
    
    float upLen = sqrtf(camera->up.x * camera->up.x + camera->up.y * camera->up.y + camera->up.z * camera->up.z);
    camera->up.x /= upLen; camera->up.y /= upLen; camera->up.z /= upLen;
}
