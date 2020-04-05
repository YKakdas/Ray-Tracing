#ifndef OBJECT_TYPES_H
#define OBJECT_TYPES_H
#include "Angel.h"
#include <string>
using namespace std;

typedef struct LightSource {
	vec3 LightPos;
	float Ir, Ig, Ib, a, b, c;
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
	vec3 center;
    float radius;
};

typedef struct ray {
	vec3 origin;
	vec3 destination;
	vec3 direction;
};
#endif // !OBJECT_TYPES_H

