#pragma once
#include "vectorMath.h"
using namespace libPRGR;

class Camera {

public:
	vector4f radio = { 0,0,0,1 };
	vector4f pos = { 0,0,0,1 };
	vector4f up = { 0,1,0,0 }; //verticalidad de la cámara
	vector4f lookAt = { 0,0,0,1 }; //punto al que mira

	vector4f rot{ 0,0,0,0 };
	vector4f lookAtOriginal = { 0,0,0,1 };

	float fovy = 60; // ángulo de apertura/visión
	float aspectRatio = 680.0f / 480.0f; //Tamaño de ventana hardcodeado
	float zNear = 0.01f;
	float zFar = 100.0f;

	float ratonAnteriorX = 0;
	float ratonAnteriorY = 0;

	Camera(vector4f pos, vector4f up, vector4f lookAt) : pos(pos), lookAt(lookAt), up(up) {
		
	}

	matrix4x4f computeViewMatrix();
	void move();
	matrix4x4f computeProjectionMatrix();
};