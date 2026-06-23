#ifndef PHYSICS_H
#define PHYSICS_H

// Declaración adelantada para evitar acoplamiento circular de cabeceras
struct Car;

// Constante global de gravedad para el entorno
#define GRAVITY_CONSTANT 18.0f

// Aplica las leyes del entorno sobre el vehículo (Fuerza G, caídas y límites de suelo)
void UpdateUniversalPhysics(struct Car *car, float dt);

#endif // PHYSICS_H
