#include "car.h"
#include "cJSON.h"
#include "raylib.h"
#include "raymath.h"
#include "input.h"
#include <math.h>
#include <stddef.h>
#include <string.h>

void InitCarFromJSON(Car *car, const char *filename) {
    memset(car, 0, sizeof(Car));
    car->position = (Vector3){ 0.0f, 1.0f, 0.0f };
    car->rotation = 0.0f;
    car->speed = 0.0f;
    car->velocityY = 0.0f;
    car->isGrounded = true;
    car->visualSlipAngle = 0.0f;
    car->angularVelocity = 0.0f;
    car->revLimiterTimer = 0.0f;
    car->lastTurnInput = 0.0f;
    car->currentGear = 0;
    car->rpm = 0.0f;
    car->shiftTimer = 0.0f;
    car->driveType = DRIVE_RWD;
    
    car->weight = 1200.0f;
    car->maxSpeed = 45.0f;      
    car->acceleration = 8.0f;   
    car->braking = 12.0f;       
    car->friction = 0.98f;      
    car->turnSpeed = 2.8f;      
    car->tireScrub = 0.85f;     
    car->weightTransferAcceleration = 6.0f;
    car->weightTransferBraking = 8.0f;
    car->bodyRollIntensity = 4.0f; 
    car->suspensionStiffness = 8.0f;
    car->maxRpm = 7000.0f;
    car->gearMaxSpeeds[0] = 10.0f;
    car->gearMaxSpeeds[1] = 20.0f;
    car->gearMaxSpeeds[2] = 30.0f;
    car->gearMaxSpeeds[3] = 38.0f;
    car->gearMaxSpeeds[4] = 45.0f;
    car->shiftDelay = 0.3f;

    char *jsonText = LoadFileText(filename);
    if (!jsonText) {
        TraceLog(LOG_WARNING, "InitCarFromJSON: No se pudo cargar %s", filename);
        return;
    }

    cJSON *json = cJSON_Parse(jsonText);
    if (!json) {
        TraceLog(LOG_WARNING, "InitCarFromJSON: JSON inválido en %s", filename);
        UnloadFileText(jsonText);
        return;
    }

    cJSON *item;
    if ((item = cJSON_GetObjectItem(json, "driveType"))) {
        if (cJSON_IsString(item)) {
            if (strcmp(item->valuestring, "FWD") == 0) car->driveType = DRIVE_FWD;
            else if (strcmp(item->valuestring, "RWD") == 0) car->driveType = DRIVE_RWD;
            else if (strcmp(item->valuestring, "AWD") == 0) car->driveType = DRIVE_AWD;
        }
    }
    
    if ((item = cJSON_GetObjectItem(json, "weight"))) car->weight = (float)item->valuedouble;
    if ((item = cJSON_GetObjectItem(json, "maxSpeed"))) car->maxSpeed = (float)item->valuedouble;
    if ((item = cJSON_GetObjectItem(json, "acceleration"))) car->acceleration = (float)item->valuedouble;
    if ((item = cJSON_GetObjectItem(json, "braking"))) car->braking = (float)item->valuedouble;
    if ((item = cJSON_GetObjectItem(json, "friction"))) car->friction = (float)item->valuedouble;
    if ((item = cJSON_GetObjectItem(json, "tireScrub"))) car->tireScrub = (float)item->valuedouble;
    if ((item = cJSON_GetObjectItem(json, "weightTransferAcceleration"))) car->weightTransferAcceleration = (float)item->valuedouble;
    if ((item = cJSON_GetObjectItem(json, "weightTransferBraking"))) car->weightTransferBraking = (float)item->valuedouble;
    if ((item = cJSON_GetObjectItem(json, "bodyRollIntensity"))) car->bodyRollIntensity = (float)item->valuedouble;
    if ((item = cJSON_GetObjectItem(json, "suspensionStiffness"))) car->suspensionStiffness = (float)item->valuedouble;
    if ((item = cJSON_GetObjectItem(json, "maxRpm"))) car->maxRpm = (float)item->valuedouble;
    
    if ((item = cJSON_GetObjectItem(json, "turnSpeed"))) {
        car->turnSpeed = (float)item->valuedouble * DEG2RAD;
    }
    
    if ((item = cJSON_GetObjectItem(json, "gearMaxSpeeds"))) {
        if (cJSON_IsArray(item)) {
            int arraySize = cJSON_GetArraySize(item);
            for (int i = 0; i < arraySize && i < 5; i++) {
                cJSON *gearItem = cJSON_GetArrayItem(item, i);
                if (gearItem && cJSON_IsNumber(gearItem)) {
                    car->gearMaxSpeeds[i] = (float)gearItem->valuedouble;
                }
            }
        }
    }

    cJSON_Delete(json);
    UnloadFileText(jsonText);
}

void UpdateCarDriving(Car *car, float dt) {
    if (!car || !car->isGrounded) return;

    // 1. INPUTS Y VARIABLES LOCALES
    bool accel = IsActionDown(INPUT_ACCELERATE);
    bool brake = IsActionDown(INPUT_BRAKE);
    bool handbrake = IsActionDown(INPUT_HANDBRAKE);
    float turnInput = 0.0f;
    if (IsActionDown(INPUT_LEFT)) turnInput = 1.0f;
    if (IsActionDown(INPUT_RIGHT)) turnInput = -1.0f;

    float absSpeed = fabsf(car->speed);
    float speedRatio = absSpeed / car->maxSpeed;
    if (speedRatio > 1.0f) speedRatio = 1.0f;
    float speedKmh = absSpeed * 3.6f;

    // MODULO: DETECCIÓN DE CORTE DE INYECCIÓN
    bool revCut = false;
    if (car->rpm >= car->maxRpm - 120.0f || car->revLimiterTimer > 0.0f) {
        car->revLimiterTimer += dt;
        if (car->revLimiterTimer > 0.07f) {
            car->revLimiterTimer = 0.0f;
        }
        if (car->revLimiterTimer > 0.035f) {
            revCut = true;
        }
    }

    // 2. DINÁMICA LONGITUDINAL
    float engineForce = 0.0f;
    if (accel) {
        if (revCut) {
            engineForce = -car->acceleration * 0.4f;
        } else {
            engineForce = car->acceleration * (1.0f - speedRatio * 0.4f);
        }
    }
    if (brake) {
        if (car->speed > 0.5f) {
            engineForce = -car->braking;
        } else if (car->speed <= 0.5f && car->speed > -8.0f) {
            engineForce = -car->acceleration * 0.6f;
        }
    }
    car->speed += engineForce * dt;

    if (!accel && !brake) {
        car->speed *= (1.0f - (car->friction * 0.1f * dt));
    }

    // 3. CAPA DE GIRO E INERCIA ANGULAR
    if (absSpeed > 0.2f) {
        float steeringSensitivity = 1.0f - (speedRatio * 0.50f); 
        float handbrakeBonus = handbrake ? 1.4f : 1.0f;
        
        // MODULO: SCANDINAVIAN FLICK
        float flickForce = 1.0f;
        if (turnInput != 0.0f && car->lastTurnInput != 0.0f && (turnInput * car->lastTurnInput < 0.0f) && absSpeed > 10.0f) {
            flickForce = 2.2f;
            car->angularVelocity += turnInput * car->turnSpeed * 1.8f;
        }

        float targetTorque = turnInput * car->turnSpeed * steeringSensitivity * handbrakeBonus * flickForce;
        car->angularVelocity += targetTorque * 12.0f * dt;
        car->angularVelocity *= (1.0f - (7.5f * dt)); 
        car->rotation += car->angularVelocity * dt * RAD2DEG;
    } else {
        car->angularVelocity *= (1.0f - (15.0f * dt));
    }
    car->lastTurnInput = turnInput;

    // 4. SISTEMA DE TRACCIÓN SELECTIVA Y COMPORTAMIENTO DE AGARRE (SLIP ANGLE)
    float targetSlipAngle = 0.0f;
    float gripSnappingSpeed = 7.5f; 
    bool isCounterSteering = (car->visualSlipAngle * turnInput < 0.0f && turnInput != 0.0f);

    if (handbrake && absSpeed > 4.0f) {
        float driftDir = turnInput != 0.0f ? turnInput : (car->visualSlipAngle > 0.0f ? 1.0f : -1.0f);
        targetSlipAngle = driftDir * 38.0f; 
        gripSnappingSpeed = 2.0f;           
        car->speed -= car->tireScrub * 0.9f * dt * (car->speed > 0.0f ? 1.0f : -1.0f);
    } 
    else if (turnInput != 0.0f && speedRatio > 0.25f) {
        if (car->driveType == DRIVE_RWD) {
            float powerOversteer = accel ? 1.6f : 1.0f;
            targetSlipAngle = turnInput * 22.0f * speedRatio * powerOversteer;
            gripSnappingSpeed = 3.5f;
            car->speed -= car->tireScrub * 0.3f * dt;
        } 
        else if (car->driveType == DRIVE_FWD) {
            float powerUndersteer = accel ? 0.2f : 1.0f;
            targetSlipAngle = turnInput * 10.0f * speedRatio * powerUndersteer;
            gripSnappingSpeed = 6.5f;
        } 
        else if (car->driveType == DRIVE_AWD) {
            targetSlipAngle = turnInput * 14.0f * speedRatio;
            gripSnappingSpeed = 5.0f;
            car->speed -= car->tireScrub * 0.15f * dt;
        }
    } 
    else {
        targetSlipAngle = 0.0f;
        gripSnappingSpeed = (car->driveType == DRIVE_FWD) ? 9.5f : 8.0f; 
    }

    if (isCounterSteering && !handbrake) {
        float stabilizationFactor = (car->driveType == DRIVE_FWD) ? 3.5f : 2.2f;
        gripSnappingSpeed *= stabilizationFactor;
        targetSlipAngle *= 0.3f;
    }

    car->visualSlipAngle = Lerp(car->visualSlipAngle, targetSlipAngle, gripSnappingSpeed * dt);

    // 5. TRASLACIÓN VECTORIAL
    float gripModifier = 0.4f;
    if (car->driveType == DRIVE_AWD) gripModifier = 0.70f;
    if (car->driveType == DRIVE_FWD) gripModifier = 0.20f;

    float moveAngle = (car->rotation * DEG2RAD) + (car->visualSlipAngle * DEG2RAD * (1.0f - gripModifier));
    car->position.x += sinf(moveAngle) * car->speed * dt;
    car->position.z += cosf(moveAngle) * car->speed * dt;

    // 6. CHASSIS ROLL Y PITCH
    float targetPitch = 0.0f;
    if (accel) targetPitch = -1.5f * speedRatio;
    if (brake && car->speed > 0.5f) targetPitch = 2.0f * speedRatio;

    float dynamicLateralG = (turnInput * 0.25f) + (car->visualSlipAngle / 38.0f) * 0.75f;
    float targetRoll = -dynamicLateralG * speedRatio * car->bodyRollIntensity;
    targetRoll = Clamp(targetRoll, -15.0f, 15.0f);
    
    car->pitch += (targetPitch - car->pitch) * 6.0f * dt;
    car->roll += (targetRoll - car->roll) * 6.0f * dt;

    // 7. MOTOR Y TRANSMISIÓN AUTOMÁTICA
    if (car->currentGear >= 0 && car->currentGear < 5) {
        float maxSpeedGear = car->gearMaxSpeeds[car->currentGear];
        if (maxSpeedGear > 0.1f) {
            car->rpm = (absSpeed / maxSpeedGear) * car->maxRpm;
            if (revCut) {
                car->rpm = car->maxRpm - 550.0f;
            }
            car->rpm = fmaxf(0.0f, fminf(car->rpm, car->maxRpm));
        }
    }

    car->shiftTimer -= dt;
    if (car->shiftTimer <= 0.0f) {
        float rpmPercent = car->rpm / car->maxRpm;
        
        if (car->currentGear < 4 && rpmPercent > 0.85f && speedKmh > car->gearMaxSpeeds[car->currentGear] * 0.7f) {
            car->currentGear++;
            car->shiftTimer = car->shiftDelay;
        }
        else if (car->currentGear > 0 && rpmPercent < 0.4f) {
            car->currentGear--;
            car->shiftTimer = car->shiftDelay * 0.5f;
        }
    }

    // 8. CLAMP DE VELOCIDAD
    if (absSpeed > car->maxSpeed) {
        car->speed = (car->speed > 0 ? car->maxSpeed : -car->maxSpeed);
    }
    if (car->speed < -8.0f) {
        car->speed = -8.0f;
    }
}
