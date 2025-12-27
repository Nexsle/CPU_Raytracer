#pragma once
#include "Hittable.h"
#include "rtweekend.h"


class sphere : public hittable
{
public:
	sphere(const point3& center, double radius) : center(center), radius(std::fmax(0, radius)) {}

	bool hit(const ray& ray, interval rayT, hitRecord& rec) const override
	{
		vec3 oc = center - ray.Origin();
		auto a = ray.Direction().LengthSquared();
		auto h = Dot(ray.Direction(), oc);
		auto c = oc.LengthSquared() - radius * radius;
		auto discriminant = h * h - a * c;

		if (discriminant < 0)
		{
			return false;
		}
	
		auto sqrtd = std::sqrt(discriminant);

		auto root = (h - sqrtd) / a;
		if (!rayT.Surounds(root))
		{
			root = (h + sqrtd) / a;
			if (!rayT.Surounds(root))
				return false; // both close and far lie outside the range
		}

		rec.t = root;
		rec.p = ray.At(rec.t);
		vec3 outwardNormal = (rec.p - center) / radius;
		rec.SetFaceNormal(ray, outwardNormal);

		return true;
	}
private:
	point3 center;
	double radius;
};