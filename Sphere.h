#pragma once
#include "Hittable.h"
#include "rtweekend.h"


class sphere : public hittable
{
public:
	//stationary sphere
	sphere(const point3& staticCenter, double radius, shared_ptr<material> mat) 
		: center(staticCenter, vec3(0,0,0)), radius(std::fmax(0, radius)), mat(mat) {}

	//Moving sphere
	sphere(const point3& center1, const point3& center2, double radius, shared_ptr<material> mat)
		: center(center1, center2-center1), radius(std::fmax(0, radius)), mat(mat) {}
	bool hit(const ray& ray, interval rayT, hitRecord& rec) const override
	{
		point3 currentCenter = center.At(ray.Time());
		vec3 oc = currentCenter - ray.Origin();
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
		vec3 outwardNormal = (rec.p - currentCenter) / radius;
		rec.SetFaceNormal(ray, outwardNormal);
		rec.mat = mat;

		return true;
	}
private:
	ray center;
	double radius;
	shared_ptr<material> mat;
};