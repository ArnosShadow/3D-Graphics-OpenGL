#version 330

uniform mat4 MVP;
uniform mat4 M;
uniform int isOutline;

attribute vec4 vPos;
attribute vec4 vColor;
attribute vec4 vNorm;
attribute vec4 vTextureCoord;

out vec4 fColor;
out vec4 fNorm;
out vec4 fPos;
out vec4 fTextureCoord;

void main() {
    vec4 pos = vPos;

    // Expande la malla si es el contorno
    if (isOutline == 1) {
        vec4 normal = normalize(inverse(transpose(M)) * vNorm);
        normal.w = 0.0;
        pos += normal * 0.03; // grosor del contorno
    }

    gl_Position = MVP * pos;
    fColor = vColor;
    fPos = M * vPos;
    fNorm = normalize(inverse(transpose(M)) * vNorm);
    fNorm.w = 0.0;
    fTextureCoord = vTextureCoord;
}
