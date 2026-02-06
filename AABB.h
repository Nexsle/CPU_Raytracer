#pragma once
#include "rtweekend.h"

//Axis align bounding box
class aabb
{
public:
	interval x, y, z;

	aabb() {};

	aabb(const interval& x, const interval& y, const interval& z)
		: x(x), y(y), z(z) 
	{
		PadToMinimums();
	}

	aabb(const point3& a, const point3& b)
	{
		//treating 2 point as extrema for the bounding box

		x = (a[0] <= b[0]) ? interval(a[0], b[0]) : interval(b[0], a[0]);
		y = (a[1] <= b[1]) ? interval(a[1], b[1]) : interval(b[1], a[1]);
		z = (a[2] <= b[2]) ? interval(a[2], b[2]) : interval(b[2], a[2]);

		PadToMinimums();
	}

	aabb(const aabb& box0, const aabb& box1)
	{
		x = interval(box0.x, box1.x);
		y = interval(box0.y, box1.y);
		z = interval(box0.z, box1.z);
	}

	const interval& AxisInterval(int n) const
	{
		if (n == 1) return y;
		if (n == 2) return z;
		return x;
	}

	bool hit(const ray& ray, interval rayT) const 
	{
		const point3& rayOrigin = ray.Origin();
		const vec3& rayDirection = ray.Direction();

		for (int axis = 0; axis < 3; axis++)
		{
			const interval& ax = AxisInterval(axis);
			const double adinv = 1.0 / rayDirection[axis];

			auto t0 = (ax.min - rayOrigin[axis]) * adinv;
			auto t1 = (ax.max - rayOrigin[axis]) * adinv;

			if (t0 < t1)
			{
				if (t0 > rayT.min) rayT.min = t0;
				if (t1 < rayT.max) rayT.max = t1;
			}
			else
			{
				if (t1 > rayT.min) rayT.min = t1;
				if (t0 < rayT.max) rayT.max = t0;
			}

			if (rayT.max <= rayT.min)
				return false;
		}

		return true;
	}

	int LongestAxis() const
	{
		if (x.Size() > y.Size())
			return x.Size() > z.Size() ? 0 : 2;
		else
			return y.Size() > z.Size() ? 1 : 2;
	}

	static const aabb empty, universe;

private:
	void PadToMinimums()
	{
		double delta = 0.0001;
		if (x.Size() < delta) x = x.Expand(delta);
		if (y.Size() < delta) y = y.Expand(delta);
		if (z.Size() < delta) z = z.Expand(delta);
	}
};

const aabb aabb::empty = aabb(interval::empty, interval::empty, interval::empty);
const aabb aabb::universe = aabb(interval::universe, interval::universe, interval::universe);