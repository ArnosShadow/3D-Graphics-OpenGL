#pragma once
#include "light.h"
#include "vectorMath.h"

class OrbitalLight : public Light {
public:
    float radius;               
    float speed;               
    float angle;                
    vector4f center;          

    OrbitalLight(vector4f pos, float I, float r, float speed, vector4f color);

    void move(double timeStep) override;
};
