#pragma once
#include "Hittable.h"
#include "rtweekend.h"
#include "AABB.h"

#include <vector>


class hittableList : public hittable
{
public:
	std::vector<shared_ptr<hittable>> objects;

	hittableList() {}
	hittableList(shared_ptr<hittable> object) { add(object); }

	void clear() { objects.clear(); }

	void add(shared_ptr<hittable> object) 
	{ 
		objects.push_back(object); 
		bbox = aabb(bbox, object->BoundingBox());
	}

	bool hit(const ray& ray, interval rayT, hitRecord& rec) const override
	{
		hitRecord tempRec;
		bool hitAnything = false;
		auto closestSoFar = rayT.max;

		for (const auto& object : objects)
		{
			if (object->hit(ray, interval(rayT.min, closestSoFar), tempRec))
			{
				hitAnything = true;
				closestSoFar = tempRec.t;
				rec = tempRec;
			}
		}

		return hitAnything;
	}

	aabb BoundingBox() const override { return bbox; }
private:
	aabb bbox;
};