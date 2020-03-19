#include "SphereIntersection.h"

float SphereIntersection::intersect(const vec3 &center, float radius, Ray &ray, bool &isInside) {
	float a = dot(ray.getDirection(), ray.getDirection());
	vec3 u = (center - ray.getOrigin());
	float b = -2.0*dot(u, ray.getDirection());
	float c = dot(u, u) - radius * radius;

	vec3 m = u - dot(u, ray.getDirection())*ray.getDirection();
	//float discriminant = b * b - 4 * a*c;
	float discriminant = 4 * (radius*radius - dot(m, m));
	if (discriminant < 0) return -1.0;
	else {
		float root1 = (-b - sqrt(discriminant)) / (2.0*a);
		float root2 = (-b + sqrt(discriminant)) / (2.0*a);

		if (root1 > 0 && root2 < 0) return root1;
		else if (root2 > 0 && root1 < 0) {
			isInside = true;
			return root2;
		}
		else if (root1 < 0 && root2 < 0) {
			isInside = true;
			return root1;
		}
		else if (root1 > 0 && root2 > 0) {
			if (root1 < root2) {
				return root1;
			}
			else {
				return root2;
			}
		}
	}
}

vec3 SphereIntersection::intersectionPoint(Ray &ray, float t) {
	return ray.getOrigin() + t * ray.getDirection();
}