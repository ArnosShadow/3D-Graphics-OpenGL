#include "light.h"
#include "vectorMath.h"
using namespace libPRGR;
void Light::move(double timeStep)
{
	// Centro de órbita (posición del objeto)
	vector4f center = posicionObjeto;
	
	// Radio de la órbita
	float radius = 10.0f;

	// Velocidad angular (en radianes/segundo)
	float angularSpeed = 3.0f; // 1 radian/segundo

	// Ángulo actual
	static float angle = 0.0f;
	angle += angularSpeed * timeStep;

	// Nueva posición en órbita (en el plano XZ)
	pos.x = center.x + radius * cos(angle);
	pos.y = center.y + 0.5f; // un poco por encima
	pos.z = center.z + radius * sin(angle);
	pos.w = 1.0f;

	// Dirección hacia el objeto
	direction = normalizar(center - pos);
}

