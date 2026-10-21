#pragma once
#define _USE_MATH_DEFINES
#include  <math.h>
#include <iostream>

namespace libPRGR {

	typedef struct {
		union {
			struct {
				float x, y, z, w;
			};
			struct{
				float r, g, b, a;
			};
			float data[4];
		};
	}vector4f ;


	typedef struct {
		union {
			float mat2D[4][4];
			float mat1[16];
			vector4f rows[4];
		};
	}matrix4x4f;



	inline vector4f make_vector4f(float x, float y, float z, float w) {
		vector4f v1 = {x,y,z,w};
		return v1;
	}

	inline vector4f normalizar(vector4f v) {
		vector4f vres;
		float magnitud = std::sqrt(v.x*v.x + v.y * v.y+ v.z * v.z);
		if (magnitud == 0) {
			vres = {0,0,0,1};
		}
		else {
			vres = {v.x / magnitud, v.y / magnitud, v.z / magnitud, v.w};
		}

		return vres;
	}
	inline vector4f operator + (vector4f v1, vector4f v2) {
		vector4f res = { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w };
		return res;
	}
	inline vector4f operator + (vector4f v1, float scalar) {
		vector4f res = { v1.x + scalar, v1.y + scalar, v1.z + scalar, v1.w };
		return res;
	}
	inline vector4f operator - (vector4f v1, vector4f v2) {
		vector4f res = { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w-v2.w };
		return res;
	}
	inline vector4f operator - (vector4f v1, float scalar) {
		vector4f res = { v1.x - scalar, v1.y - scalar, v1.z - scalar, v1.w  };
		return res;
	}
	inline float operator* (vector4f v1, vector4f v2) {
		float res = 0;
		res = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
		return res;
	}
	inline vector4f operator% (vector4f v1, vector4f v2) {

		vector4f res = { v1.y * v2.z - v1.z * v2.y,  v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x, 0 };
		return res;
	}


	inline float operator/ (vector4f v1, vector4f v2) {
		float res = 0;
		if (v2.x != 0) res += v1.x / v2.x;
		if (v2.y != 0) res += v1.y / v2.y;
		if (v2.z != 0) res += v1.z / v2.z;
		if (v2.w != 0) res += v1.w / v2.w;
		return res;
	}
	inline vector4f operator/ (vector4f v1, float scalar) {
		vector4f res = {};

		if (scalar == 0) { throw ("Error"); }
		res = {v1.x/scalar, v1.y / scalar ,v1.z / scalar ,v1.w / scalar };

		return res;
	}
	inline vector4f operator* (vector4f v1, float scalar) {
		vector4f res = {};
		
		res = { v1.x * scalar, v1.y * scalar ,v1.z * scalar ,v1.w * scalar };

		return res;
	}

	inline matrix4x4f identidad() {
		return matrix4x4f{ .rows {{1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1} } };
	}

	inline matrix4x4f matriz_translacion(float x, float y, float z) {
		matrix4x4f res = identidad();
		res.mat2D[0][3] = x;
		res.mat2D[1][3] = y;
		res.mat2D[2][3] = z;
		return res;
	}
	inline matrix4x4f matriz_scale(float x, float y, float z) {
		matrix4x4f res = identidad();
		res.mat2D[0][0]=x;
		res.mat2D[1][1]=y;
		res.mat2D[2][2]=z;

		return res;
	}
	inline float toRadians(float angle) {
		return M_PI * angle / 180.0f;
	}

	inline matrix4x4f matrixrotX(float anglex) {
		float radX = toRadians(anglex);
		matrix4x4f rotX = { .rows {{1, 0, 0, 0}, {0, cos(radX), -sin(radX), 0}, {0, sin(radX), cos(radX), 0}, {0, 0, 0, 1}} };
		return rotX;
	}
	inline matrix4x4f matrixrotY(float angley) {
		float radY = toRadians(angley);
		matrix4x4f rotY = { .rows {{cos(radY), 0, sin(radY), 0}, {0, 1, 0, 0}, {-sin(radY), 0, cos(radY), 0}, {0, 0, 0, 1}} };
		return rotY;
	}
	inline matrix4x4f matrixrotZ(float anglez) {
		float radZ = toRadians(anglez);
		matrix4x4f rotZ = { .rows {{cos(radZ), -sin(radZ), 0, 0}, {sin(radZ), cos(radZ), 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}} };
		return rotZ;
	}
	inline matrix4x4f operator* (matrix4x4f m1, matrix4x4f m2) {
		matrix4x4f res = {};
		for (int j = 0; j < 4; j++) {
			for (int i = 0; i < 4; i++) {
				for (int k = 0; k < 4; k++) {
					res.mat2D[i][j] += m1.mat2D[i][k] * m2.mat2D[k][j];

				}

			}

		}
		return res;
	}
	inline matrix4x4f matriz_rotacion(float anglex, float angley, float anglez) {
		return matrixrotX(anglex) * matrixrotY(angley) * matrixrotZ(anglez);
	}

	inline vector4f operator* (matrix4x4f m1, vector4f v1) {
		vector4f vres = {0,0,0,0};
		for (int j = 0; j < 4; j++) {
			vres.data[j] = m1.rows[j]*v1;
		}
		return vres;
	}
	
	inline matrix4x4f operator* (matrix4x4f m1, float scalar) {
		matrix4x4f res = {};
		for (int j = 0; j < 4; j++) {
			for (int i = 0; i < 4; i++) {
				res.mat2D[j][i] = m1.mat2D[j][i] * scalar;
			}
		}
		return res;
	}
	inline matrix4x4f operator+ (matrix4x4f m1, matrix4x4f m2) {
		matrix4x4f res = {};

		for (int j = 0; j < 4; j++) {
			for (int i = 0; i < 4; i++) {
				res.mat2D[j][i] = m1.mat2D[j][i] + m2.mat2D[j][i];
			}
		}
		return res;
	}
	inline matrix4x4f operator- (matrix4x4f m1, matrix4x4f m2) {
		matrix4x4f res = {};

		for (int j = 0; j < 4; j++) {
			for (int i = 0; i < 4; i++) {
				res.mat2D[j][i] = m1.mat2D[j][i] - m2.mat2D[j][i];
			}
		}
		return res;
	}

	inline matrix4x4f transpuesta(matrix4x4f m1) {
		matrix4x4f res = {};

		for (int j = 0; j < 4; j++) {
			for (int i = 0; i < 4; i++) {
				res.mat2D[i][j] = m1.mat2D[j][i];
			}
		}

		return res;
	}

	inline float determinante3x3(float menorMatriz[3][3]) {
		return (menorMatriz[0][0] * (menorMatriz[1][1] * menorMatriz[2][2] - menorMatriz[1][2] * menorMatriz[2][1])
			- menorMatriz[0][1] * (menorMatriz[1][0] * menorMatriz[2][2] - menorMatriz[1][2] * menorMatriz[2][0])
			+ menorMatriz[0][2] * (menorMatriz[1][0] * menorMatriz[2][1] - menorMatriz[1][1] * menorMatriz[2][0]));
	}
	inline matrix4x4f adjunto(matrix4x4f m1) {

		matrix4x4f adj = {};
		float res = 0;
		int subfila = 0, subcolumna = 0;
		float submatriz[3][3]{};
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				subfila = 0;
				for (int fila = 0; fila < 4; fila++) {
					if (i == fila) continue;
					subcolumna = 0;
					for (int columna=0; columna < 4; columna++) {
						if (j == columna) continue;
						submatriz[subfila][subcolumna] = m1.mat2D[fila][columna];
						subcolumna++;
						
					}
					subfila++;
				}

				res += ((j % 2 ? -1 : 1)) * m1.mat2D[0][j] * determinante3x3(submatriz);
				adj.mat2D[i][j] = res;
			}

		}

		return adj;

	}
	inline float determinante4x4(matrix4x4f m1) {
		float res = 0;
		int fila=0, columna = 0;
		float menorMatriz[3][3]{};
		for (int j = 0; j < 4; j++) {
			fila = 0;
			for (int i = 0; i < 4; i++) {
				columna=0;
				if (i == 0) continue;
				for (int k = 0; k < 4; k++) {
					if (k == j) continue;
					menorMatriz[fila][columna] = m1.mat2D[i][k];

					columna++;
				}
				fila++;
			}
			res+=((j%2 ? -1: 1))*m1.mat2D[0][j]* determinante3x3(menorMatriz);
		}
		return res;
	}
	inline matrix4x4f operator/ (matrix4x4f m1, float escalar) {
		matrix4x4f res = {};

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				res.mat2D[i][j] = m1.mat2D[i][j]  / escalar;
			}
		}

		return res;
	}
	inline matrix4x4f inversa(matrix4x4f m1) {
		//Inversa por Laplace
		matrix4x4f res = {};

		float det = determinante4x4(m1);

		if (det ==0) {
			res= identidad();
		}
		else {
			res= transpuesta(adjunto(m1)) / determinante4x4(m1);
		}

		return res;	
	}

	inline vector4f operator* (float scalar, vector4f v1) {
		v1.x = v1.x * scalar;
		v1.y = v1.y * scalar;
		v1.z = v1.z * scalar;
		v1.w = v1.w * scalar;


		return v1;
	}
	inline matrix4x4f inversaGauss(matrix4x4f A) {
	
		matrix4x4f res = {};
		matrix4x4f I = identidad();
		float aumentada[4][8]{};

		//Paso 1: Agrupamos las dos matrices.
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				aumentada[i][j] = A.mat2D[i][j];
				aumentada[i][j + 4] = I.mat2D[i][j];
			}
		}
		//Paso 2: Eliminamos 
		for (int i = 0; i < 4; i++) {
			if (aumentada[i][i] == 0) continue;
			float inicial=aumentada[i][i];

			for(int j = 0; j < 8; j++) {
				aumentada[i][j] /= inicial;
			}
			for (int k = 0; k < 4; k++) {
				if (k == i) continue;
				for (int l = 0; l < 8; l++) {
					aumentada[k][l] -= inicial * aumentada[i][l];
				}
			}
		}

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				res.mat2D[i][j] = aumentada[i][j + 4];
			}
		}

		return res;
	
	}


	inline vector4f make_quaternion(float x, float y, float z, float angle) {
		float semiAngulo = angle / 2;
		float sinSemiAngulo = sin(semiAngulo);

		x = x * sinSemiAngulo;
		y = y * sinSemiAngulo;
		z = z * sinSemiAngulo;
		float w = cos(semiAngulo);

		return make_vector4f(x,y,z,w);
	}

	inline matrix4x4f make_rotate(vector4f quaternion) {
		matrix4x4f matriz_rotacion;

		float x=quaternion.x;
		float y = quaternion.y;
		float z = quaternion.z;
		float w = quaternion.w;
		matriz_rotacion={ .rows {{1 - 2 * y * y - 2 * z * z,(2 * x * y - 2 * w * z),(2 * x * z + 2 * w * y),0},
								 {(2 * x * y + 2 * w * z), (1 - 2 * x * x - 2 * z * z), (2 * y * z - 2 * w * x), 0},
								 {(2 * x * z - 2 * w * y), (2 * y * z + 2 * w * x), (1 - 2 * x * x - 2 * y * y), 0},
								 {0,0,0,1}}};

		return matriz_rotacion;
	}

	inline float distance(const vector4f& v1, const vector4f& v2) {
		float dx = v2.x - v1.x;
		float dy = v2.y - v1.y;
		float dz = v2.z - v1.z;
		return std::sqrt(dx * dx + dy * dy + dz * dz);
	}

	inline std::ostream& operator << (std::ostream& os, vector4f v1) {
		os <<  "{" << v1.data[0] << "," << v1.data[1] << "," << v1.data[2] << "," << v1.data[3] << "}\n";
		return os;
	}

	
};

