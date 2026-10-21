#pragma once
#include "common.h"
#include "vectorMath.h"
using namespace libPRGR;

typedef enum {
    sphere, AABB, OBB
}BVType;

struct Triangle {
    vector4f v0, v1, v2;
};



//particulas, unidades mínimas de colisión
class BV {
public:
    BVType type;
    vector<Triangle> triangles;
    bool Nodecorativo = true;
    BV() { type = sphere; };
    virtual bool collision(BV* bv2) = 0;
    virtual void update(matrix4x4f& mat, vector4f size) = 0;
    virtual vector4f getSize() = 0;
    virtual vector4f getCenter() = 0;

    //Implementacion del algoritmo de Möller.
    bool triangleTriangleCollision(const Triangle& t1, const Triangle& t2);


};

class SphereBV : public BV {

public:
    vector4f center = { 0,0,0,1 };
    vector4f centerOrigin = { 0,0,0,1 };

    vector4f velocity = { 0,0,0,1 };
    vector4f aceleracion = { 0,-9.81f,0,1 };
    float radiusOrigin = 0;
    float radius = 0;



    SphereBV() { type = sphere; };

    SphereBV(vector4f center, float radious, bool decorativo = true);
    bool collision(BV* bv2);
    void update(matrix4x4f& mat, vector4f size);

    vector4f getSize() { return { radius * 2.0f,radius * 2.0f, radius * 2.0f, 0 }; };
    vector4f getCenter() { return center; };

};


//Collision de AABB.
//Axis-Aligned Bounding Box
class AABBBV : public BV {

public:
    vector4f center = { 0,0,0,1 };
    vector4f centerOrigin = { 0,0,0,1 };
    float distanceOrigin = 0;
    float distancia = 0;

    //Vertice inferior izquierdo
    vector4f A = {0,0,0,1};

    //Vector opuesto el superior derecho
    vector4f C = { 0,0,0,1 };

    AABBBV() { type = AABB; };

    AABBBV(vector4f center, vector4f A, vector4f C, bool Nodecorativo);

    bool collision(BV* bv2);
    void update(matrix4x4f& mat, vector4f size);

    vector4f getSize() { return { C.x - A.x,C.y - A.y, C.z - A.z, 0 }; };
    vector4f getCenter() { return center; };

};

//Collision de OBB.
class OBBBV : public BV {

public:
    vector4f center = { 0,0,0,1 };
    vector4f centerOrigin = { 0,0,0,1 };
    vector4f vectoresOrientacion[3]; 
    float width, height, depth;

    //Vertice inferior izquierdo
    vector4f A = { 0,0,0,1 };

    //Vector opuesto el superior derecho
    vector4f C = { 0,0,0,1 };
    OBBBV() { type = OBB; };

    OBBBV(vector4f center, float width, float height, float depth, vector4f* vectoresOrientacion, bool Nodecorativo);

    vector<vector4f> getNormals() {
        vector<vector4f> normals;

        // Las normales corresponden a los vectores de orientación
        normals.push_back(vectoresOrientacion[0]);  // Normal de la cara en dirección X
        normals.push_back(vectoresOrientacion[1]);  // Normal de la cara en dirección Y
        normals.push_back(vectoresOrientacion[2]);  // Normal de la cara en dirección Z

        // Negar los vectores de orientación para las caras opuestas
        normals.push_back(-1*vectoresOrientacion[0]);  // Normal de la cara opuesta a la X
        normals.push_back(-1*vectoresOrientacion[1]);  // Normal de la cara opuesta a la Y
        normals.push_back(-1*vectoresOrientacion[2]);  // Normal de la cara opuesta a la Z

        return normals;
    }
    vector<vector4f> getVertices() {
        // Similar a la función getVertices del SAT: los 8 vértices de la OBB
        vector<vector4f> vertices(8); // Calcular los vértices de la OBB en función del centro, orientación y dimensiones
        
        vector4f halfDimensions = { width / 2.0f, height / 2.0f, depth / 2.0f, 0.0f };

        vertices[0] = center + halfDimensions.x * vectoresOrientacion[0]  + halfDimensions.y * vectoresOrientacion[1] + halfDimensions.z * vectoresOrientacion[2];
        vertices[1] = center + halfDimensions.x * vectoresOrientacion[0] + halfDimensions.y * vectoresOrientacion[1] - halfDimensions.z * vectoresOrientacion[2];
        vertices[2] = center + halfDimensions.x * vectoresOrientacion[0] - halfDimensions.y * vectoresOrientacion[1] + halfDimensions.z * vectoresOrientacion[2];
        vertices[3] = center + halfDimensions.x * vectoresOrientacion[0] - halfDimensions.y * vectoresOrientacion[1] - halfDimensions.z * vectoresOrientacion[2];
        vertices[4] = center - halfDimensions.x * vectoresOrientacion[0] + halfDimensions.y * vectoresOrientacion[1] + halfDimensions.z * vectoresOrientacion[2];
        vertices[5] = center - halfDimensions.x * vectoresOrientacion[0] + halfDimensions.y * vectoresOrientacion[1] - halfDimensions.z * vectoresOrientacion[2];
        vertices[6] = center - halfDimensions.x * vectoresOrientacion[0] - halfDimensions.y * vectoresOrientacion[1] + halfDimensions.z * vectoresOrientacion[2];
        vertices[7] = center - halfDimensions.x * vectoresOrientacion[0] - halfDimensions.y * vectoresOrientacion[1] - halfDimensions.z * vectoresOrientacion[2];
        return vertices;
    }


    bool volumesSeSolapan(BV* bv2);
    bool collision(BV* bv2);
    bool isVerticesOnSameSide(const vector4f& normal, const vector4f& planePoint, OBBBV* other);
    void update(matrix4x4f& mat, vector4f size);


    vector4f getSize() { return { C.x - A.x,C.y - A.y, C.z - A.z, 0 }; };
    vector4f getCenter() { return center; };

};

class VBHNode
{
public:
    vector4f max = { -numeric_limits<float>::max(),-numeric_limits<float>::max(),-numeric_limits<float>::max(),1 };
    vector4f min = { numeric_limits<float>::max(), numeric_limits<float>::max(), numeric_limits<float>::max(),1 };

    VBHNode* parent = nullptr;
    BV* volumeTest = nullptr;
    vector<BV*> particles;
    VBHNode* sons[2] = { nullptr,nullptr };

    VBHNode() {}
    void subdivide();//generar dos hijos a partir del volumeTest, repartir particulas entre hijos, ajustar volumen

    bool collision(VBHNode* node);//dos nodos colisionan si alguno de sus hijos colisionan

    void update(matrix4x4f& mat, vector4f size);

    //añade un BV y recalcula volumeTest
    void addParticle(BV* newParticle);

    void addParticleAABB(BV* newParticle);
    void addParticleOBB(BV* newParticle);
    void addParticleSphere(BV* newParticle);

};

class VBH { //jerarquía

public:

    VBHNode* root = nullptr;
    VBH() { root = new VBHNode(); };
    void addParticle(BV* newParticle) { root->addParticle(newParticle); };
    void subdivide() { root->subdivide(); };
    void update(matrix4x4f& mat, vector4f size) { root->update(mat, size); };

};
