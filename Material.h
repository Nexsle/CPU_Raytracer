#pragma once

#include "Hittable.h"

class material
{
public:
	virtual ~material() = default;

	virtual bool scatter(const ray& rayIn, const hitRecord& rec, color& attenuation, ray& scattered)const
	{
		return false;
	}
};

class lambertian : public material
{
public:
	lambertian(const color& albedo) :albedo(albedo) {}

	bool scatter(const ray& rayIn, const hitRecord& rec, color& attenuation, ray& scattered) const override
	{
		auto scatterDirection = rec.normal + RandomUnitVector();

		if (scatterDirection.NearZero())
		{
			scatterDirection = rec.normal;
		}

		scattered = ray(rec.p, scatterDirection);
		attenuation = albedo;
		return true;
	 }


private:
	color albedo;
};

class metal : public material
{
public:
	metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1){}

	bool scatter(const ray& rayIn, const hitRecord& rec, color& attenuation, ray& scattered) const override
	{
		vec3 reflected = Reflect(rayIn.Direction(), rec.normal);
		reflected = UnitVector(reflected) + (fuzz * RandomUnitVector());
		scattered = ray(rec.p, reflected);
		attenuation = albedo;
		return Dot(scattered.Direction(), rec.normal) > 0;

	}

private: 
	color albedo;
	double fuzz;
};
