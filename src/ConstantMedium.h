#pragma once

#include "Hittable.h"
#include "Material.h"
#include "Texture.h"

class constantMedium : public hittable
{
public:
	constantMedium(shared_ptr<hittable> boundary, double density, shared_ptr<texture> tex)
		: boundary(boundary), negInvDensity(-1 / density), phaseFunction(make_shared<isotropic>(tex))
	{}

	constantMedium(shared_ptr<hittable> boundary, double density, const color& albedo)
		:boundary(boundary), negInvDensity(-1 / density), phaseFunction(make_shared<isotropic>(albedo))
	{}

	bool hit(const ray& ray, interval rayT, hitRecord& rec) const override
	{
		hitRecord rec1, rec2;

		if (!boundary->hit(ray, interval::universe, rec1))
			return false;

		if (!boundary->hit(ray, interval(rec1.t + 0.0001, infinity), rec2))
			return false;

		if (rec1.t < rayT.min) rec1.t = rayT.min;
		if (rec2.t > rayT.max) rec2.t = rayT.max;

		if (rec1.t >= rec2.t)
			return false;

		if (rec1.t < 0)
			rec1.t = 0;

		auto rayLength = ray.Direction().Length();
		auto distanceInsideBoundary = (rec2.t - rec1.t) * rayLength;
		auto hitDistance = negInvDensity * std::log(RandomDouble());

		if (hitDistance > distanceInsideBoundary)
			return false;

		rec.t = rec1.t + hitDistance / rayLength;
		rec.p = ray.At(rec.t);
		
		rec.normal = vec3(1, 0, 0);
		rec.frontFace = true;
		rec.mat = phaseFunction;

		return true;
	}

	aabb BoundingBox() const override { return boundary->BoundingBox(); }

private:
	shared_ptr<hittable> boundary;
	double negInvDensity;
	shared_ptr<material> phaseFunction;

};