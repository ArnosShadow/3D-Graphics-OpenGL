#include "OrbitalLight.h"
OrbitalLight::OrbitalLight(vector4f centerPos, float I, float radius, float speed, vector4f color)
    : Light(puntoLuz, centerPos, color, I, { 0,0,0,0 }),
    center(centerPos), radius(radius), speed(speed), angle(0)
{
    // La posición inicial se establece con respecto al centro y el radio
    pos.x = center.x + radius;
    pos.z = center.z;
}

void OrbitalLight::move(double timeStep) {
    if (type != puntoLuz) return; // Solo se mueve si es punto de luz

    angle += speed * timeStep;

    pos.x = center.x + radius * cos(angle);
    pos.z = center.z + radius * sin(angle);
    pos.y = center.y; // opcional: mantener misma altura
}
