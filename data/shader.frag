#version 330 core

in vec4 fColor;
in vec4 fNorm;
in vec4 fPos;
in vec4 fTextureCoord;

uniform sampler2D textureColor;

// Material
uniform float Kd;
uniform float Ks;
uniform int shinny;

// Luces
#define MAX_LIGHTS 10
uniform int numLights;
uniform vec4 lightPos[MAX_LIGHTS];
uniform vec4 lightColor[MAX_LIGHTS];
uniform float Ik[MAX_LIGHTS];
uniform int lightType[MAX_LIGHTS];
uniform vec4 lightDirection[MAX_LIGHTS];

// Cámara
uniform vec4 cameraPos;

// Toon Shader Control
uniform int isOutline;

out vec4 fragColor;

#define direccional 0
#define puntoLuz 1

void main() {
    if (isOutline == 1) {
        fragColor = vec4(0.0, 0.0, 0.0, 1.0); // contorno negro
        return;
    }

    vec4 texColor = texture(textureColor, fTextureCoord.xy);
    if (texColor.a < 0.1) discard;

    float ambient = 0.2;
    vec4 finalColor = vec4(0.0);

    for (int i = 0; i < numLights; ++i) {
        vec4 L = (lightType[i] == direccional) 
               ? normalize(-lightDirection[i]) 
               : normalize(lightPos[i] - fPos);

        vec4 N = normalize(fNorm);
        float diff = max(dot(N, L), 0.0);

        // Toon shading: escalones
        float toonDiff;
        if (diff > 0.95) toonDiff = 1.0;
        else if (diff > 0.5) toonDiff = 0.7;
        else if (diff > 0.25) toonDiff = 0.4;
        else toonDiff = 0.1;

        vec4 R = normalize(reflect(-L, N));
        vec4 V = normalize(cameraPos - fPos);
        float spec = pow(max(dot(R, V), 0.0), shinny);

        float intensity = ambient + Ik[i] * (Kd * toonDiff + Ks * spec);
        finalColor += texColor * intensity * lightColor[i];
    }

    fragColor = finalColor;
}
