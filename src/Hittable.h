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


class translate : public hittable
{
public:
	translate(shared_ptr<hittable> object, const vec3& offset) : object(object), offset(offset) 
	{
		bbox = object->BoundingBox() + offset;
	}


	bool hit(const ray& r, interval rayT, hitRecord& rec) const override
	{
		ray offsetR(r.Origin() - offset, r.Direction(), r.Time());

		if (!object->hit(offsetR, rayT, rec))
			return false;

		rec.p += offset;

		return true;
	}

	aabb BoundingBox() const override { return bbox; }

private:
	vec3 offset;
	shared_ptr<hittable> object;
	aabb bbox;
};

class rotateY : public hittable
{
public:
	rotateY(shared_ptr<hittable> object, double angle) : object(object)
	{
		auto radian = DegreeToRadians(angle);

		sinTheta = std::sin(radian);
		cosTheta = std::cos(radian);
		bbox = object->BoundingBox();

		point3 min(infinity, infinity, infinity);
		point3 max(-infinity, -infinity, -infinity);

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				for (int k = 0; k < 2; k++)
				{
					auto x = i * bbox.x.max + (1 - i) * bbox.x.min;
					auto y = j * bbox.y.max + (1 - j) * bbox.y.min;
					auto z = k * bbox.y.max + (1 - k) * bbox.y.min;
					
					auto newx = cosTheta * x + sinTheta * z;
					auto newz = -sinTheta * x + cosTheta * z;

					vec3 tester(newx, y, newz);

					for (int c = 0; c < 3; c++)
					{
						min[c] = std::fmin(min[c], tester[c]);
						max[c] = std::fmax(max[c], tester[c]);
					}
				}
			}
		}

		bbox = aabb(min, max);
	}


	bool hit(const ray& r, interval rayT, hitRecord& rec) const override
	{
		//transform ray from world space to object space

		auto origin = point3(
			(cosTheta * r.Origin().x()) - (sinTheta * r.Origin().z()),
			r.Origin().y(),
			(sinTheta * r.Origin().x()) + (cosTheta * r.Origin().z())
		);

		auto direction = point3(
			(cosTheta * r.Direction().x()) - (sinTheta * r.Direction().z()),
			r.Direction().y(),
			(sinTheta * r.Direction().x()) + (cosTheta * r.Direction().z())
		);

		ray rotatedR(origin, direction, r.Time());

		if (!object->hit(rotatedR, rayT, rec))
			return false;

		rec.p = point3(
			(cosTheta * rec.p.x()) - (sinTheta * rec.p.z()),
			rec.p.y(),
			(sinTheta * rec.p.x()) + (cosTheta * rec.p.z())
		);

		rec.normal = point3(
			(cosTheta * rec.normal.x()) - (sinTheta * rec.normal.z()),
			rec.normal.y(),
			(sinTheta * rec.normal.x()) + (cosTheta * rec.normal.z())
		);

		return true;
	}

	aabb BoundingBox() const override { return bbox; }

private:
	shared_ptr<hittable> object;
	double sinTheta;
	double cosTheta;
	aabb bbox;
};