#ifndef RAY_H
#define RAY_H

#include "ObjectTypes.h"
class Ray {
private:
	vec3 origin;
	vec3 destination;
	vec3 direction;
public:
	Ray(vec3 &origin, vec3 &destination) {
		this->origin = origin;
		this->destination = destination;
		direction = normalize(destination - origin);
	}
	const vec3 getDirection() {
		return this->direction;
	}
	const vec3 getOrigin() {
		return this->origin;
	}
};
#endif
