#ifndef RAY_TRACING_H
#define RAY_TRACING_H
#include "Angel.h"
#include "Ray.h"

class RayTracing {
public:
	RayTracing(){}
	vec3 trace(Ray &ray);
	bool isVisible(vec3 point, LightSource lightSource);
	vec3 phong(vec3 P, LightSource lightSource, vec3 normal, SceneObj sceneObj);
	void writePixel(int i, int j, vec3 color);
};

#endif // !RAY_TRACING_H
