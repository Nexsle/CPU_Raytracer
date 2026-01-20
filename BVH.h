#pragma once
#include "AABB.h"
#include "hittable.h"
#include "Hittable_List.h"

#include <algorithm>

class bvhNode : public hittable
{
public:
	bvhNode(hittableList list) : bvhNode(list.objects, 0, list.objects.size()) {}

	bvhNode(std::vector<shared_ptr<hittable>>& objects, size_t start, size_t end)
	{
		bbox = aabb::empty;
		for (size_t objectIndex = start; objectIndex < end; objectIndex++)
			bbox = aabb(bbox, objects[objectIndex]->BoundingBox());

		int axis = bbox.LongestAxis();

		auto comparator = (axis == 0) ? BoxXCompare
			: (axis == 1) ? BoxYCompare
			:BoxZCompare;

		size_t objectSpan = end - start;

		if (objectSpan == 1) {
			left = right = objects[start];
		}
		else if (objectSpan == 2)
		{
			left = objects[start];
			right = objects[start + 1];
		}
		else {
			std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);

			auto mid = start + objectSpan / 2;
			left = make_shared<bvhNode>(objects, start, mid);
			right = make_shared<bvhNode>(objects, mid, end);
		}

	}

	bool hit(const ray& ray, interval rayT, hitRecord& rec) const override
	{
		if (!bbox.hit(ray, rayT))
			return false;

		bool hitLeft = left->hit(ray, rayT, rec);
		bool hitRight = right->hit(ray, interval(rayT.min, hitLeft ? rec.t : rayT.max), rec);

		return hitLeft || hitRight;
	}

	aabb BoundingBox() const override { return bbox; }

private:
	shared_ptr<hittable> left;
	shared_ptr<hittable> right;
	aabb bbox;


	static bool BoxCompare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis_index)
	{
		auto aAxisInterval = a->BoundingBox().AxisInterval(axis_index);
		auto bAxisInterval = b->BoundingBox().AxisInterval(axis_index);
		return aAxisInterval.min < bAxisInterval.min;
	}

	static bool BoxXCompare(const shared_ptr<hittable> a, const shared_ptr<hittable> b)
	{
		return BoxCompare(a, b, 0);
	}
	static bool BoxYCompare(const shared_ptr<hittable> a, const shared_ptr<hittable> b)
	{
		return BoxCompare(a, b, 1);
	}
	static bool BoxZCompare(const shared_ptr<hittable> a, const shared_ptr<hittable> b)
	{
		return BoxCompare(a, b, 2);
	}
};