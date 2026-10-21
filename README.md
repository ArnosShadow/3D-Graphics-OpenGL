# Escena 3D interactiva — C++ & OpenGL

**Proyecto académico de Programación Gráfica · U-tad**  
**Tecnologías:** C++20 · OpenGL · GLSL · GLFW · GLAD · Visual Studio

Aplicación gráfica que construye y actualiza una **escena tridimensional interactiva** con cámara controlable, geometría cargada desde archivos, iluminación, sombreado estilizado y detección de colisiones. El proyecto combina la parte de *rendering* de OpenGL con geometría, transformaciones y una simulación física sencilla.

> **Enfoque:** proyecto de aprendizaje y experimentación con gráficos 3D, no motor comercial ni biblioteca de uso general.

## 🎬 Demostración en vídeo


https://github.com/user-attachments/assets/bb54a1ff-72e5-4e36-aa92-c3f1eff93aa7


La demostración puede enseñar la exploración de la escena con la cámara, las fuentes de luz y la interacción entre la esfera y los elementos del entorno.

<img width="627" height="466" alt="image" src="https://github.com/user-attachments/assets/342b2b6b-4a9d-4369-956b-41868b13082b" />



## Características técnicas

- **Renderizado 3D con OpenGL:** gestión de objetos y envío de geometría a GPU mediante VAO, VBO e índices; matrices de modelo, vista y proyección.
- **Cámara interactiva:** orientación con ratón y desplazamiento con teclado; comprobación de colisiones con obstáculos para limitar el movimiento.
- **Geometría y recursos:** lectura de mallas en formato `.fiis`, materiales, texturas y shaders desde `data/`.
- **Iluminación:** soporte de luces direccionales y puntuales, incluidas luces orbitales de distintos colores.
- **Shaders GLSL:** cálculo de iluminación difusa y especular y lógica de *toon shading* por niveles; incluye una pasada de contorno en el renderizador.
- **Colisiones y movimiento:** volúmenes esféricos y cajas orientadas (*OBB*), integración de gravedad y respuesta de rebote básica para la esfera.
- **Bucle de ejecución:** actualización de objetos, física, cámara, luces y renderizado de cada fotograma.

## Demostración de conceptos

| Área | Implementación en el proyecto |
| --- | --- |
| Gráficos 3D | OpenGL, geometría, buffers, cámara y transformaciones |
| Matemáticas | Vectores y matrices 4×4, proyecciones y normales |
| Shading | GLSL, iluminación difusa/especular y cuantización de luz (*toon*) |
| Física básica | Actualización temporal, gravedad y rebote de una esfera |
| Colisiones | Pruebas entre volúmenes esféricos y cajas orientadas |
| Ingeniería C++ | Clases para objetos, cámara, materiales, luces y renderizado |

## Controles

Según la implementación de `libprgr/Camera.cpp`:

| Entrada | Acción |
| --- | --- |
| **Ratón** | Orientar la cámara |
| **↑ / ↓** | Avanzar / retroceder en la dirección de mirada |
| **← / →** | Desplazamiento lateral relativo a la cámara |
| **W / S** | Desplazamiento en el eje Z global |
| **A / D** | Desplazamiento en el eje X global |

> Los controles experimentales `Z`, `C`, `Q`, `E` y espacio de `Object3D.cpp` aparecen comentados en la versión compartida y **no** forman parte de los controles activos.

## Estructura del proyecto

```text
ProgGrafica_2024/
├── MainPRGR_2024.cpp          # Construcción de la escena y configuración de luces
├── ProgGrafica_2024.vcxproj   # Proyecto de Visual Studio
├── ProgGrafica_2024.vcxproj.filters
├── libprgr/
│   ├── Render.*               # Contexto OpenGL, buffers y bucle de renderizado
│   ├── Camera.*               # Cámara y controles
│   ├── Object3D.*             # Objetos, transformaciones y física
│   ├── BV.*                   # Volúmenes y pruebas de colisión
│   ├── Light.*, OrbitalLight.*
│   └── ...                    # Shaders, texturas, materiales y matemáticas
└── data/
    ├── shader.vert           # Shader de vértices
    ├── shader.frag           # Shader de fragmentos
    ├── *.fiis                # Recursos de geometría
    └── *.png / *.jpg        # Texturas
```

Los archivos `.fiis` y los shaders de `data/` son **necesarios para ejecutar la escena**: no son archivos temporales y deben versionarse.

## Requisitos

- **Windows** con **Visual Studio 2022** y la carga de trabajo **Desarrollo de escritorio con C++**.
- Compilador **MSVC v143** y soporte de **C++20** (configuración `x64` del proyecto).
- GPU/controlador con soporte para la versión de OpenGL y GLSL utilizada.
- Bibliotecas **GLFW**, **GLAD** y **GLM**, configuradas en las rutas de inclusión del proyecto.

### Dependencias locales

El `.vcxproj` incluido busca las dependencias en estas carpetas relativas al directorio del proyecto:

```text
../externalLibs/glfw/inc
../externalLibs/glfw/lib
../externalLibs/glad/inc
../externalLibs/glm/inc
```

Además enlaza `glfw3.lib`. **Estas dependencias no vienen en el ZIP** y sus rutas pueden requerir adaptación en cada equipo. En concreto, el proyecto utiliza la cabecera `glad/gl.h`.

## Compilar y ejecutar con Visual Studio

1. Abre `ProgGrafica_2024.vcxproj` en **Visual Studio 2022**.
2. Selecciona **x64** y la configuración **Debug** o **Release**.
3. Instala/configura las dependencias y revisa en **Propiedades del proyecto → Directorios de VC++** las rutas de `Include` y `Library`.
4. En **Propiedades → Depuración → Directorio de trabajo**, usa `$(ProjectDir)` para que se encuentren las rutas `data/...`.
5. Compila con **Ctrl + Mayús + B** y ejecuta con **F5** o **Ctrl + F5**.

> El proyecto procede de un entorno académico concreto. Las rutas de bibliotecas están definidas para ese entorno; no se garantiza que compile sin configurarlas en una instalación nueva.

## Problemas habituales

| Síntoma | Qué comprobar |
| --- | --- |
| No encuentra `GLFW/glfw3.h`, `glad/gl.h` o GLM | Rutas de inclusión y versiones de dependencias |
| Error de enlace de `glfw3.lib` | Ruta de librerías y arquitectura x64 |
| No encuentra mallas, texturas o shaders | Directorio de trabajo `$(ProjectDir)` y carpeta `data/` |
| Error al compilar `shader.vert` | En GLSL 330, los atributos de entrada del *vertex shader* se declaran con `in` en lugar de `attribute` |

## Estado del proyecto

Es una **práctica académica de gráficos 3D**. Incluye prototipos y partes de código susceptibles de revisión, como la portabilidad de dependencias, la validación de shaders y el orden de algunas operaciones de renderizado. La intención de este repositorio es **mostrar los conceptos implementados y su evolución**.

## Autoría y atribuciones

**Francisco José Jaraba Estévez**  
Doble Grado en Ingeniería del Software y Física Computacional · U-tad

Si parte de la infraestructura `libprgr/`, las mallas o las texturas procede de material docente o terceros, debe identificarse su origen y comprobarse el permiso de redistribución antes de publicar el repositorio. No se asigna una licencia de reutilización a esos materiales sin verificarlo.
