#include "BV.h"

SphereBV::SphereBV(vector4f center, float radius,bool Nodecorativo)
    :center(center), radius(radius)
{
    type = sphere;
    this->Nodecorativo = Nodecorativo;
    centerOrigin = center;
    radiusOrigin = radius;
}

bool SphereBV::collision(BV* bv2)
{
    if (!bv2->Nodecorativo) return false;

    if (bv2->type == sphere) {
        SphereBV* sph2 = static_cast<SphereBV*>(bv2);
        return (sph2->radius + radius) > distance(sph2->center, center);
    }

    if (bv2->type == AABB) {
        AABBBV* aabb = static_cast<AABBBV*>(bv2);
        vector4f closestPoint;

        // Clamp cada componente de la esfera al interior del AABB
        closestPoint.x = std::max(aabb->A.x, std::min(center.x, aabb->C.x));
        closestPoint.y = std::max(aabb->A.y, std::min(center.y, aabb->C.y));
        closestPoint.z = std::max(aabb->A.z, std::min(center.z, aabb->C.z));
        closestPoint.w = 1;

        float dist = distance(center, closestPoint);
        return dist < radius;
    }

    if (bv2->type == OBB) {
        OBBBV* obb = static_cast<OBBBV*>(bv2);
        vector4f dir = center - obb->center;
        float local[3];

        for (int i = 0; i < 3; i++) {
            local[i] = (dir * obb->vectoresOrientacion[i]);
            float halfLength = (i == 0 ? obb->width : (i == 1 ? obb->height : obb->depth)) / 2.0f;
            local[i] = max(-halfLength, min(local[i], halfLength));
        }

        vector4f closestPoint = obb->center;
        for (int i = 0; i < 3; i++) {
            closestPoint = closestPoint + local[i] * obb->vectoresOrientacion[i];
        }

        return distance(center, closestPoint) < radius;
    }


    return false;
}


void SphereBV::update(matrix4x4f& mat, vector4f size)
{
    this->center = mat * centerOrigin;
    this->radius = size.x * radiusOrigin;
}

void VBHNode::subdivide()
{
    //averiguar centro geométrico  del volumeTest
    vector4f center = volumeTest->getCenter();
    //averiguar tamaño volumeTest
    vector4f size = { (max.x - min.x) / 2.0f,(max.y - min.y) / 2.0f,(max.z - min.z) / 2.0f,1 };
    //falta borrar si ya existían
    sons[0] = new VBHNode();
    sons[1] = new VBHNode();

    //elegir eje mayor 
    // mover particulas a izquierda/derecha de eje mayor 
    if (size.x >= size.y && size.x >= size.z) //eje mayor X
    {
        for (auto& p : particles)
        {
            if (p->getCenter().x > center.x) //al hijo 1
            {
                sons[1]->addParticle(p);
            }
            else //al hijo 0
                sons[0]->addParticle(p);
        }
    }
    else if (size.y >= size.x && size.y >= size.z)//eje mayor Y
    {

        for (auto& p : particles)
        {
            if (p->getCenter().y > center.y) //al hijo 1
            {
                sons[1]->addParticle(p);
            }
            else //al hijo 0
                sons[0]->addParticle(p);
        }
    }
    else //eje mayor Z
    {

        for (auto& p : particles)
        {
            if (p->getCenter().z > center.z) //al hijo 1
            {
                sons[1]->addParticle(p);
            }
            else //al hijo 0
                sons[0]->addParticle(p);
        }
    }

    //ajustar 
    //si ambos tienen partículas, subdividir
    if (sons[0]->particles.size() > 0 && sons[1]->particles.size() > 0)
    {
        sons[0]->subdivide();
        sons[1]->subdivide();
    }
    else//borrar hijos si no he subdividido
    {
        delete sons[0];
        delete sons[1];
        sons[0] = nullptr;
        sons[1] = nullptr;
    }
}

bool VBHNode::collision(VBHNode* node)
{
    bool test = false;
    //si el nodo colisiona conmigo
    if (!volumeTest->Nodecorativo || !node->volumeTest->Nodecorativo) return false;
    test = volumeTest->collision(node->volumeTest);

    if (test)
    {
        test = false;
        if (sons[0] && node->sons[0]) //si ambos tienen hijos: test todos hijos con todos hijos
            for (int i = 0; i < 2; i++)
                for (int j = 0; j < 2; j++)
                {
                    test |= sons[i]->collision(node->sons[j]);
                }
        else if (sons[0] && !node->sons[0])//nodo hoja vs hijos
        {
            for (int j = 0; j < 2; j++)
            {
                test |= sons[j]->collision(node);

            }
        }
        else if (!sons[0] && node->sons[0])// hijos vs nodo hoja
        {
            for (int j = 0; j < 2; j++)
            {
                test |= this->collision(node->sons[j]);

            }
        }
        //else ambos son hojas, acaban
        else
            test = true;


    }

    return test;
}

void VBHNode::update(matrix4x4f& mat, vector4f size)
{
    this->volumeTest->update(mat, size);
    if (sons[0])
    {
        sons[0]->update(mat, size);
        sons[1]->update(mat, size);
    }
}
//actualiza volumeTest
void VBHNode::addParticle(BV* newParticle)
{
    if (newParticle->type == sphere) {
        addParticleSphere(newParticle);
    }
    if (newParticle->type == AABB) {
        addParticleAABB(newParticle);
    }
    if (newParticle->type == OBB) {
        addParticleOBB(newParticle);

    }
}

void VBHNode::addParticleAABB(BV* newParticle)
{
    if (volumeTest == nullptr) volumeTest = new AABBBV();
    //averiguar centro geométrico  del volumeTest
    vector4f center = newParticle->getCenter();
    vector4f distancia = newParticle->getSize() / 2.0f;

    //actualizar max y min
    max.x = max.x < center.x + distancia.x ? center.x + distancia.x : max.x;
    max.y = max.y < center.y + distancia.y ? center.y + distancia.y : max.y;
    max.z = max.z < center.z + distancia.z ? center.z + distancia.z : max.z;

    min.x = min.x > center.x - distancia.x ? center.x - distancia.x : min.x;
    min.y = min.y > center.y - distancia.y ? center.y - distancia.y : min.y;
    min.z = min.z > center.z - distancia.z ? center.z - distancia.z : min.z;

    // Actualizar el volumen envolvente del nodo
    ((AABBBV*)volumeTest)->A = min;
    ((AABBBV*)volumeTest)->C = max;
    ((AABBBV*)volumeTest)->center = (min + max) / 2.0f;

    particles.push_back(newParticle);

}

void VBHNode::addParticleOBB(BV* newParticle)
{
    if (volumeTest == nullptr) volumeTest = new OBBBV();
    vector4f center = newParticle->getCenter();
    vector4f distancia = newParticle->getSize() / 2.0f;

    // Actualizar max y min según las dimensiones de la OBB
    max.x = max.x < center.x + distancia.x ? center.x + distancia.x : max.x;
    max.y = max.y < center.y + distancia.y ? center.y + distancia.y : max.y;
    max.z = max.z < center.z + distancia.z ? center.z + distancia.z : max.z;

    min.x = min.x > center.x - distancia.x ? center.x - distancia.x : min.x;
    min.y = min.y > center.y - distancia.y ? center.y - distancia.y : min.y;
    min.z = min.z > center.z - distancia.z ? center.z - distancia.z : min.z;

    // Actualizar el volumen envolvente del nodo
    ((OBBBV*)volumeTest)->vectoresOrientacion[0] = vector4f({ 1, 0, 0, 0 }); // Eje X
    ((OBBBV*)volumeTest)->vectoresOrientacion[1] = vector4f({ 0, 1, 0, 0 }); // Eje Y
    ((OBBBV*)volumeTest)->vectoresOrientacion[2] = vector4f({ 0, 0, 1, 0 }); // Eje Z
    ((OBBBV*)volumeTest)->center = (min + max) / 2.0f;

    particles.push_back(newParticle);

}


void VBHNode::addParticleSphere(BV* newParticle) {
    if (volumeTest == nullptr) volumeTest = new SphereBV();

    particles.push_back(newParticle);

    //averiguar centro geométrico  del volumeTest
    vector4f center = newParticle->getCenter();
    //averiguar tamaño volumeTest
    vector4f radius = newParticle->getSize() / 2.0f;

    //actualizar max y min
    max.x = max.x < center.x + radius.x ? center.x + radius.x : max.x;
    max.y = max.y < center.y + radius.y ? center.y + radius.y : max.y;
    max.z = max.z < center.z + radius.z ? center.z + radius.z : max.z;

    min.x = min.x > center.x - radius.x ? center.x - radius.x : min.x;
    min.y = min.y > center.y - radius.y ? center.y - radius.y : min.y;
    min.z = min.z > center.z - radius.z ? center.z - radius.z : min.z;

    ((SphereBV*)volumeTest)->centerOrigin = (max + min) / 2.0f;
    ((SphereBV*)volumeTest)->centerOrigin.w = 1;
    ((SphereBV*)volumeTest)->radiusOrigin = distance(max, min) / 2.0f;
    ((SphereBV*)volumeTest)->center = ((SphereBV*)volumeTest)->centerOrigin;
    ((SphereBV*)volumeTest)->radius = ((SphereBV*)volumeTest)->radiusOrigin;
}

AABBBV::AABBBV(vector4f center, vector4f A, vector4f C, bool Nodecorativo): center(center), A(A), C(C)
{
    type = AABB;
    this->Nodecorativo = Nodecorativo;
    centerOrigin = center;
}

bool AABBBV::collision(BV* bv2) {
    if (bv2->type != AABB) return false;
    AABBBV* aabb2 = static_cast<AABBBV*>(bv2);

    return !(C.x < aabb2->A.x || A.x > aabb2->C.x ||
        C.y < aabb2->A.y || A.y > aabb2->C.y ||
        C.z < aabb2->A.z || A.z > aabb2->C.z);
}


void AABBBV::update(matrix4x4f& mat, vector4f size)
{

    this->center = mat * centerOrigin;
    this->A = mat * A;
    this->C = mat * C;

}

OBBBV::OBBBV(vector4f center, float width, float height, float depth, vector4f* vectoresOrientacion, bool Nodecorativo):
    center(center), width(width), height(height), depth(depth)
{
    this->Nodecorativo = Nodecorativo;
    this->vectoresOrientacion[0] = vectoresOrientacion[0];  // Eje X
    this->vectoresOrientacion[1] = vectoresOrientacion[1];  // Eje Y
    this->vectoresOrientacion[2] = vectoresOrientacion[2];  // Eje Z
	type = OBB;
}



// Verificar si dos OBBs colisionan usando las normales de las caras (planos)
bool OBBBV::volumesSeSolapan(BV* bv2) {
    if (bv2->type != OBB) return false;
    OBBBV* oBB2 = static_cast<OBBBV*>(bv2);

    // Obtener las normales de las dos OBBs
    vector<vector4f> normals1 = OBBBV::getNormals();
    vector<vector4f> normals2 = oBB2->getNormals();

    // Verificar intersección con los planos de cada OBB
    for (const auto& normal : normals1) {
        if (!isVerticesOnSameSide(normal, center, oBB2)) {
            return false;  // No hay colisión
        }
    }

    for (const auto& normal : normals2) {
        if (!isVerticesOnSameSide(normal, oBB2->center, this)) {
            return false;  // No hay colisión
        }
    }

    return true;  // Si todos los planos se intersectan, hay colisión

}

bool OBBBV::collision(BV* bv2) {
    if (!this->volumesSeSolapan(bv2)) return false;

    // Ahora chequeo detallado triángulo a triángulo
    for (const Triangle& t1 : this->triangles) {
        for (const Triangle& t2 : bv2->triangles) {
            if (triangleTriangleCollision(t1, t2)) return true;
        }
    }

    return false;
}

// Función para verificar si los vértices de la otra OBB caen del lado del plano
bool OBBBV::isVerticesOnSameSide(const vector4f& normal, const vector4f& planePoint, OBBBV* other) {
    vector<vector4f> otherVertices = other->getVertices();

    // Determinar en qué lado del plano se encuentran los vértices de la otra OBB
    float d = -(planePoint * normal);  // d es la constante del plano
    bool allOnOneSide = true;
        
    for (const auto& vertex : otherVertices) {
        // Proyección de los vértices de la otra OBB sobre el plano
        float distance = (vertex * normal) + d;
        if (distance > 0) {
            allOnOneSide = false; // Si al menos un vértice está del otro lado, no hay colisión
        }
    }

    // Verificar si los vértices están dentro de los límites de la cara
    // Asegurarse de que las proyecciones estén dentro de las dimensiones de la OBB en X, Y y Z
    bool withinLimits = true;
    for (const auto& vertex : otherVertices) {
        if (((vertex - center) * vectoresOrientacion[1]) < -height / 2.0f || ((vertex - center) * vectoresOrientacion[1]) > height / 2.0f) {
            withinLimits = false;
            break;
        }
        if (((vertex - center) * vectoresOrientacion[2]) < -depth / 2.0f || ((vertex - center) * vectoresOrientacion[2]) > depth / 2.0f) {
            withinLimits = false;
            break;
        }
    }

    return allOnOneSide && withinLimits; // Si está dentro de los límites, hay intersección
}

void OBBBV::update(matrix4x4f& mat, vector4f size)
{

    this->center = mat * centerOrigin;
    this->vectoresOrientacion[0] = mat * vector4f({ 1, 0, 0, 0 });  // Transformar el eje X
    this->vectoresOrientacion[1] = mat * vector4f({ 0, 1, 0, 0 });  // Transformar el eje Y
    this->vectoresOrientacion[2] = mat * vector4f({ 0, 0, 1, 0 }); //Transformar el eje Z

}


// algoritmo de Möller-Trumbore
bool BV::triangleTriangleCollision(const Triangle& t1, const Triangle& t2) {
    auto rayIntersectsTriangle = [](const vector4f& orig, const vector4f& dir, const Triangle& tri, float& t) -> bool {
        const float EPSILON = 1e-6f;
        vector4f edge1 = tri.v1 - tri.v0;
        vector4f edge2 = tri.v2 - tri.v0;
        vector4f h = (dir% edge2);
        float a = (edge1* h);

        if (abs(a) < EPSILON) return false;  // El rayo es paralelo

        float f = 1.0f / a;
        vector4f s = orig - tri.v0;
        float u = f * (s* h);
        if (u < 0.0f || u > 1.0f) return false;

        vector4f q = (s % edge1);
        float v = f * (dir* q);
        if (v < 0.0f || u + v > 1.0f) return false;

        t = f * (edge2* q);
        return t > EPSILON;
        };

    float t;

    // Comprobamos si algún lado del triángulo t1 intersecta el triángulo t2
    vector<vector4f> edgesT1 = { t1.v1 - t1.v0, t1.v2 - t1.v1, t1.v0 - t1.v2 };
    vector<vector4f> originsT1 = { t1.v0, t1.v1, t1.v2 };

    for (int i = 0; i < 3; ++i) {
        if (rayIntersectsTriangle(originsT1[i], edgesT1[i], t2, t))
            return true;
    }

    // Y viceversa: lados del triángulo t2 contra t1
    vector<vector4f> edgesT2 = { t2.v1 - t2.v0, t2.v2 - t2.v1, t2.v0 - t2.v2 };
    vector<vector4f> originsT2 = { t2.v0, t2.v1, t2.v2 };

    for (int i = 0; i < 3; ++i) {
        if (rayIntersectsTriangle(originsT2[i], edgesT2[i], t1, t))
            return true;
    }

    return false;
}

