#ifndef RAY_TRACING_H
#define RAY_TRACING_H
#include "Angel.h"
#include "ObjectTypes.h"
#include <vector>

class RayTracing {
public:
	RayTracing(){}
	vec3 trace(ray r,int depth);
	bool isVisible(vec3 point, LightSource lightSource, vector<SceneObj> sceneObjects);
	vec3 phong(vec3 P, vec3 normal, SceneObj sceneObj);
	void outputToPPM(vec3** arr);
	vec3 intersectionPoint(ray r, float t);
	float intersect(const vec3 &center, float radius, ray r, bool &isInside);
};

#endif // !RAY_TRACING_H
