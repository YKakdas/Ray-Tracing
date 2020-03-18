#ifndef RAY_TRACING_H
#define RAY_TRACING_H
#include "Angel.h"
#include "Ray.h"
#include <vector>

class RayTracing {
public:
	RayTracing(){}
	vec3 trace(Ray &ray);
	bool isVisible(vec3 point, LightSource lightSource, vector<SceneObj> sceneObjects);
	vec3 phong(vec3 P, LightSource lightSource, vec3 normal, SceneObj sceneObj);
	void outputToPPM(vec3** arr);
};

#endif // !RAY_TRACING_H
