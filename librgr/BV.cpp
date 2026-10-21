#include "BV.h"

SphereBV::SphereBV(vector4f center, float radius)
    :center(center), radius(radius)
{
    type = sphere;
    centerOrigin = center;
    radiusOrigin = radius;
}

bool SphereBV::collision(BV* bv2)
{
    SphereBV* sph2 = (SphereBV*)bv2;
    return (sph2->radius + radius) > (distance(sph2->center, center));
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
