#include <iostream>
#define GLAD_BIN
#include "libprgr/common.h"
#include "libprgr/vectorMath.h"
#include "libprgr/Render.h"
#include "libprgr/light.h"
#include "libprgr/OrbitalLight.h"

#include "libprgr/EventManager.h"
using namespace libPRGR;


int main(int argc, char** argv)
{
    Render* render = new Render(640, 480);
    render->camera = new Camera({ 0,0.5f,-8,1 }, { 0,1,0,0 }, { 0,0,0,1 });

    Light* lightEstatica = new Light(Light::direccional, { 1,1,1,1 }, { 1,1,1,1 }, 1.0f, { 0,0,1,0 });

    Object3D* esfera = new Object3D("data/esfera.fiis");
    esfera->position = { 0, 6, 0, 1 };
    esfera->mat->kd = 1.0f;
    esfera->mat->ks = 0.5f;
    esfera->mat->shinny = 64;
    esfera->volume = new SphereBV(esfera->position, 1, true);

    render->putObject(esfera);

    // Crear objetos separados por cara
    Object3D* paredNorte = new Object3D("data/paredNorte.fiis");
    //Object3D* paredSur = new Object3D("data/paredSur.fiis");
    Object3D* paredEste = new Object3D("data//paredEste.fiis");
    Object3D* paredOeste = new Object3D("data/paredOeste.fiis");
    Object3D* suelo = new Object3D("data/suelo.fiis");

    // Aplicar el mismo material a cada uno
    std::vector<Object3D*> paredes = { paredNorte, /*paredSur,*/ paredEste, paredOeste, suelo };

    for (Object3D* pared : paredes) {
        // Material
        pared->mat->kd = 1.0f;
        pared->mat->ks = 0.5f;
        pared->mat->shinny = 64;

        // Bounding box a partir de vertexList
        vector4f min = pared->vertexList[0].posicion;
        vector4f max = pared->vertexList[0].posicion;

        for (const vertex_t& v : pared->vertexList) {
            min.x = std::min(min.x, v.posicion.x);
            min.y = std::min(min.y, v.posicion.y);
            min.z = std::min(min.z, v.posicion.z);

            max.x = std::max(max.x, v.posicion.x);
            max.y = std::max(max.y, v.posicion.y);
            max.z = std::max(max.z, v.posicion.z);
        }

        // Calcular centro y dimensiones
        vector4f center = {
            (min.x + max.x) / 2.0f,
            (min.y + max.y) / 2.0f,
            (min.z + max.z) / 2.0f,
            1.0f
        };

        float width = max.x - min.x;
        float height = max.y - min.y;
        float depth = max.z - min.z;

        // Ejes orientados (asumimos planos con ejes básicos)
        vector4f orientacion[3] = {
            {1, 0, 0, 0},  // eje X
            {0, 1, 0, 0},  // eje Y
            {0, 0, 1, 0}   // eje Z
        };

        // Crear volumen
        pared->volume = new OBBBV(center, width, height, depth, orientacion, true);

        render->putObject(pared);
    }

    Light* luzOrbitalRoja = new OrbitalLight({ 0, 0, 1, 1 }, 2.0f, 3.0f, 1.0f, { 1, 0, 0, 1 }); // Rojo
    Light* luzOrbitalVerde = new OrbitalLight({ 0.25f, 0, -0.75f, 1 }, 2.0f, 3.0f, 1.5f, { 0, 1, 0, 1 }); // Verde
    Light* luzOrbitalAzul = new OrbitalLight({ 0.75f, 0, -0.25f, 1 }, 2.0f, 3.0f, 0.75f, { 0, 0, 1, 1 }); // Azul

    render->lucesAsociadas[esfera] = luzOrbitalRoja;
    render->lucesAsociadas[esfera] = luzOrbitalVerde;
    render->lucesAsociadas[esfera] = luzOrbitalAzul;

    render->putLight(luzOrbitalRoja);
    render->putLight(luzOrbitalVerde);
    render->putLight(luzOrbitalAzul);
    render->putLight(lightEstatica);
    render->mainLoop();

    for (Object3D* pared : paredes) {
        render->removeObject(pared);
        delete pared;
    }
    render->removeObject(esfera);
    delete esfera;

    return 0;
}