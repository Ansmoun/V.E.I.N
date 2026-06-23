#include "physics.h"
#include "car.h"
#include <stddef.h>
#include <math.h>

void UpdateUniversalPhysics(struct Car *car, float dt) {
    if (car == NULL) return;

    // 1. GRAVEDAD MODIFICADA POR EL PESO (Sensación Inercial Arcade)
    if (!car->isGrounded) {
        // Establecemos un chasis base promedio de 1200kg como modificador neutral (1.0)
        float weightModifier = car->weight / 1200.0f;
        
        // Acotamos los límites para evitar que un camión caiga como un meteorito 
        // o un coche ultra-ligero flote como un globo.
        if (weightModifier < 0.75f) weightModifier = 0.75f;
        if (weightModifier > 1.35f) weightModifier = 1.35f;

        // Aceleramos la caída libre aplicando el peso idealizado
        car->velocityY -= (GRAVITY_CONSTANT * weightModifier) * dt;
    }

    // 2. INTEGRACIÓN EN EL EJE VERTICAL
    car->position.y += car->velocityY * dt;

    // 3. TERCERA LEY DE NEWTON: COLISIÓN DE SUELO CON REBOTE ELÁSTICO
    if (car->position.y <= 0.0f) {
        car->position.y = 0.0f; // Reubicamos en la superficie

        // Solo rebotamos si la velocidad de caída es considerable (fuerza umbral)
        // Evitamos rebotes microscópicos infinitos que hagan temblar el coche en ralentí.
        if (car->velocityY < -1.5f) {
            
            // COEFICIENTE DE RESTITUCIÓN (Elasticidad de la suspensión)
            // Calculamos cuánta fuerza se devuelve en sentido opuesto.
            // A mayor suspensionStiffness, la reacción es más dura y el rebote más vivo.
            float restitution = 0.15f * (car->suspensionStiffness / 10.0f);
            
            // Limitamos la restitución máxima al 40% de la energía original 
            // para que se estabilice rápido tras un par de botes cortos y no salte eternamente.
            if (restitution > 0.40f) restitution = 0.40f;

            // ¡ACCIÓN Y REACCIÓN! Invertimos el signo de la velocidad de caída y la mitigamos.
            car->velocityY = -car->velocityY * restitution;
            
            // Crucial: Como la velocidad ahora apunta hacia ARRIBA, el coche NO está en el suelo.
            car->isGrounded = false; 
        } 
        else {
            // El coche ha disipado toda la energía vertical, se queda firmemente adherido
            car->velocityY = 0.0f;
            car->isGrounded = true;
        }
    }
}
