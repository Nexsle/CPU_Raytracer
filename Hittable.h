#pragma once
#include "rtweekend.h"
#include "AABB.h"

class material;

class hitRecord
{
public:
	point3 p;
	vec3 normal;
	shared_ptr<material> mat;
	double t;
	double u; // Texture coords
	double v;
	bool frontFace;

	void SetFaceNormal(const ray& r, const vec3& outwardNormal)
	{
		//set the hit record normal vector
		//NOTE: the parameter 'outwardNormal' is assumed to have unit length

		frontFace = Dot(r.Direction(), outwardNormal) < 0; // ray is outside the sphere
		normal = frontFace ? outwardNormal : -outwardNormal;
	}
};


class hittable
{
public:
	virtual ~hittable() = default;

	virtual bool hit(const ray& ray, interval rayT, hitRecord& rec) const = 0;

	virtual aabb BoundingBox() const = 0;
};