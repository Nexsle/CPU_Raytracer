#pragma once
#include "Hittable.h"
#include "rtweekend.h"


class sphere : public hittable
{
public:
	//stationary sphere
	sphere(const point3& staticCenter, double radius, shared_ptr<material> mat) 
		: center(staticCenter, vec3(0,0,0)), radius(std::fmax(0, radius)), mat(mat) 
	{
		auto rvec = vec3(radius, radius, radius);
		bbox = aabb(staticCenter - rvec, staticCenter + rvec);
	}

	//Moving sphere
	sphere(const point3& center1, const point3& center2, double radius, shared_ptr<material> mat)
		: center(center1, center2-center1), radius(std::fmax(0, radius)), mat(mat) 
	{
		auto rvec = vec3(radius, radius, radius);
		aabb box1(center.At(0) - rvec, center.At(0) + rvec);
		aabb box2(center.At(1) - rvec, center.At(1) + rvec);
		bbox = aabb(box1, box2);

	}


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

	aabb BoundingBox() const override { return bbox; }

private:
	ray center;
	double radius;
	shared_ptr<material> mat;
	aabb bbox;
};