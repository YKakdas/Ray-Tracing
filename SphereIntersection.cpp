#include "SphereIntersection.h"

float SphereIntersection::intersect(const vec3 &center, float radius, Ray &ray) {
	float a = dot(ray.getDirection(), ray.getDirection());
	vec3 u = (center - ray.getOrigin());
	float b = -2.0*dot(u, ray.getDirection());
	float c = dot(u, u) - radius * radius;

	float discriminant = b * b - 4 * a*c;
	if (discriminant < 0) return discriminant;
	else {
		float root1 = (-b - sqrt(discriminant)) / (2.0*a);
		float root2 = (-b + sqrt(discriminant)) / (2.0*a);
		
		if (root1 < root2) {
			return root1;
		}
		else {
			return root2;
		}
	}
}

vec3 SphereIntersection::intersectionPoint(Ray &ray, float t) {
	return ray.getOrigin() + t * ray.getDirection();
}