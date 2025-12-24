#pragma once
#include "Vec3.h"

class ray
{
public:
	ray() {}

	ray(const vec3& origin, const vec3& direction) : orig(origin), dir(direction) {}

	const point3& Origin() const { return orig; }
	const vec3& Direction() const { return dir; }

	//ray is function P(t)=A+tb
	//where P is a 3D position along a line in 3D. A is the ray origin and b is the ray direction
	point3 at(double t)
	{
		return orig + t * dir;
	}

private:
	point3 orig;
	vec3 dir;
};