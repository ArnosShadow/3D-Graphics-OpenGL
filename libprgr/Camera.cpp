#include "Camera.h"
#include "EventManager.h"

matrix4x4f Camera::computeViewMatrix()
{
    matrix4x4f view = identidad();
    vector4f f = normalizar(lookAt - pos);
    vector4f r = normalizar(f % normalizar(up));
    vector4f u = normalizar(r % f);
    vector4f p = { -(r * pos), -(u * pos), (f * pos), 1 };
    

    view.rows[0] = r;
    view.rows[1] = u;
    view.rows[2] =  (-1.0f)* f;
    view.mat2D[0][3] = p.x;
    view.mat2D[1][3] = p.y;
    view.mat2D[2][3] = p.z;
    return view;
}

void Camera::move() {
    float raton_x = EventManager::mouseState.x;
    float raton_y = EventManager::mouseState.y;

    matrix4x4f resultado;

    /* Primera forma de mover la camara
    vector4f resultado = { 180 * raton_x / 640 , 180 * raton_y / 480, 0,1 };


    cout << resultado<< endl;
    resultado.x = toRadians(resultado.x);
    resultado.y = toRadians(resultado.y);

    lookAt.x = resultado.x;
    lookAt.y = resultado.y;
    */

    //Averiguamos direccion
    float velX = ratonAnteriorX - raton_x;
    float velY = ratonAnteriorY - raton_y;

    rot.x += velY * 0.02f;
    rot.y += velX * 0.02f;

    resultado = matriz_translacion(pos.x, pos.y, pos.z) * matriz_rotacion(rot.x, rot.y, 0);
    lookAt = resultado * (lookAtOriginal - radio);


    vector4f adelante = { cos(toRadians(rot.x)) * sin(toRadians(rot.y)),sin(toRadians(rot.x)), cos(toRadians(rot.x)) * cos(toRadians(rot.y)),0 };
    vector4f laterales = { cos(toRadians(rot.y)),0,-sin(toRadians(rot.y)),0 };

    adelante = normalizar(adelante);
    laterales = normalizar(laterales);


    lookAt = adelante + pos;

    float speed = 0.1f;
    adelante = speed * adelante;
    laterales = speed * laterales;




    if (EventManager::keyState[GLFW_KEY_UP]) {
        pos = pos + adelante;
    }
    if (EventManager::keyState[GLFW_KEY_DOWN]) {
        pos = pos - adelante;
    }
    if (EventManager::keyState[GLFW_KEY_LEFT]) {
        pos = pos + laterales;
    }
    if (EventManager::keyState[GLFW_KEY_RIGHT]) {
        pos = pos - laterales;
    }


    if (EventManager::keyState[GLFW_KEY_W]) { pos.z += 0.1f; }
    if (EventManager::keyState[GLFW_KEY_S]) { pos.z -= 0.1f; }
    if (EventManager::keyState[GLFW_KEY_A]) { pos.x += 0.1f; }
    if (EventManager::keyState[GLFW_KEY_D]) { pos.x -= 0.1f; }


    //Cambiar el valor del Anterior
    ratonAnteriorX = raton_x;
    ratonAnteriorY = raton_y;
}


matrix4x4f Camera::computeProjectionMatrix()
{
    matrix4x4f projection=identidad();

    double fovy = toRadians(this->fovy);
    projection.mat2D[0][0] = 1.0f / (aspectRatio *std::tan(fovy * 0.5f));
    projection.mat2D[1][1] = 1.0f / std::tan(fovy * 0.5f);
    projection.mat2D[2][2] = -(zFar + zNear) / (zFar - zNear);
    projection.mat2D[2][3] = 2.0f*(zFar * zNear) / (zFar - zNear);
    projection.mat2D[3][2] = -1;
    projection.mat2D[3][3] = 1;
    return projection;
}
