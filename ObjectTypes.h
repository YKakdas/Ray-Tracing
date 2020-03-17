#ifndef OBJECT_TYPES_H
#define OBJECT_TYPES_H
#include "Angel.h"
#include <string>
using namespace std;
typedef struct LightSource {
	float x, y, z, Ir, Ig, Ib, a, b, c;
};

typedef struct Pigment {
	string type;
	float r, g, b;
};

typedef struct Surface {
	float ka, kd, ks, shineness, kr;
};

typedef struct Camera {
	vec3 eye, at, up;
	float fovy;
};

typedef struct SceneObj {
	int pigmentNum;
	int surfaceNum;
	string type;
	float x, y, z, radius;
};
#endif // !OBJECT_TYPES_H

