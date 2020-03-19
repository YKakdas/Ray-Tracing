#ifndef SPHERE_INTERSECTION_H
#define SPHERE_INTERSECTION_H

#include "Ray.h"
#include "Angel.h"
class SphereIntersection {
public:
	SphereIntersection(){}
	float intersect(const vec3 &center, float radius,Ray &ray,bool &isInside);
	vec3 intersectionPoint(Ray &ray, float t);
};
#endif // !SPHERE_INTERSECTION_H
