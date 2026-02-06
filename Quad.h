#pragma once

#include "Hittable.h"

class quad : public hittable
{
public:
	quad(const point3& Q, const vec3& u, const vec3& v, shared_ptr<material> mat)
		:Q(Q), u(u), v(v), mat(mat) 
	{
		auto n = Cross(u, v);
		normal = UnitVector(n);
		D = Dot(normal, Q);
		w = n / Dot(n, n);

		SetBoundingBox();
	}

	virtual void SetBoundingBox()
	{
		auto bboxDiagonal1 = aabb(Q, Q + u + v);
		auto bboxDiagonal2 = aabb(Q + u, Q + v);

		bbox = aabb(bboxDiagonal1, bboxDiagonal2);

	}

	aabb BoundingBox() const override { return bbox; }

	bool hit(const ray& ray, interval rayT, hitRecord& rec) const override
	{
		auto denom = Dot(normal, ray.Direction());

		//ray paralel to plane
		if (std::fabs(denom) < 1e-8)
			return false;

		auto t = (D - Dot(normal, ray.Origin())) / denom;
		if (!rayT.Contains(t))
			return false;

		auto intersection = ray.At(t);
		vec3 planarHitptVector = intersection - Q;
		auto alpha = Dot(w, Cross(planarHitptVector, v));
		auto beta = Dot(w, Cross(u, planarHitptVector));

		if (!IsInterior(alpha, beta, rec))
			return false;


		rec.t = t;
		rec.p = intersection;
		rec.mat = mat;
		rec.SetFaceNormal(ray, normal);

		return true;
	}

	virtual bool IsInterior(double a, double b, hitRecord& rec) const
	{
		interval unitInterval = interval(0, 1);

		if (!unitInterval.Contains(a) || !unitInterval.Contains(b))
			return false;

		rec.u = a;
		rec.v = b;
		return true;
	}

private:
	point3 Q;
	vec3 u, v;
	shared_ptr<material> mat;
	aabb bbox;
	vec3 normal;
	double D;
	vec3 w;
};