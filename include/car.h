#ifndef CAR_H
#define CAR_H

#include "raylib.h"

typedef enum {
    DRIVE_FWD,
    DRIVE_RWD,
    DRIVE_AWD
} DriveType;

typedef struct Car {
    Vector3 position;
    float rotation;
    float speed;
    float velocityY;
    bool isGrounded;
    
    // Suspensión y visuales
    float pitch;
    float roll;
    float targetPitch;
    float targetRoll;

    // Configuración mecánica básica
    float weight;
    float maxSpeed;
    float acceleration;
    float braking;
    float friction;
    float turnSpeed;
    float tireScrub;
    float weightTransferAcceleration;
    float weightTransferBraking;
    float bodyRollIntensity;
    float suspensionStiffness;
    float angularVelocity;
	float revLimiterTimer;
	float lastTurnInput;

    // Transmisión dinámica (Escalonamiento Sawtooth)
    int currentGear;
    float rpm;
    float maxRpm;            // Dinámico: modificable por el JSON o mejoras
    float gearMaxSpeeds[5];
    float shiftDelay;        // Tiempo muerto al cambiar de marcha (ej: 0.5s)
    float shiftTimer;        // Temporizador de control de embrague
	DriveType driveType;     // Tipo de tracción (FWD, RWD, AWD)
    float visualSlipAngle;

} Car;

void InitCarFromJSON(Car *car, const char *filename);
void UpdateCarDriving(Car *car, float dt);
void DrawCarHUD(const Car *car); // Nueva función para renderizar la interfaz

#endif
