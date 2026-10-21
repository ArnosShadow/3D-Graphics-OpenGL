#pragma once
#include "common.h"
#include "vectorMath.h"

using namespace libPRGR;
class Light {
public:

	typedef enum {
		direccional,
		puntoLuz
	}lightTypes_e;


	vector4f pos = { 0,0,0,1 };
	vector4f posicionObjeto = { 0,0,0,0 };
	vector4f rot = { 0,0,0,0 };
	float I = 1.0f; //Intensidad
	float ka, kd, ks;
    vector4f color = { 0.5f,0,0,1 };

    lightTypes_e type = direccional;
    vector4f direction = { 0,0,0,0 };


	Light(lightTypes_e type, vector4f pos, vector4f color, float I, vector4f direction = { 0,0,0,0 }) :
		type(type), pos(pos), color(color), I(I), direction(direction)
	{
	};
	virtual void move(double timeStep);
};