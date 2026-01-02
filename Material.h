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


class dielectric : public material
{
public:
	dielectric(double refractionIndex) : refractionIndex(refractionIndex) {}

	bool scatter(const ray& rayIn, const hitRecord& rec, color& attenuation, ray& scattered) const override
	{
		attenuation = color(1.0, 1.0, 1.0);
		double ri = rec.frontFace ? (1.0 / refractionIndex) : refractionIndex;

		vec3 unitDirection = UnitVector(rayIn.Direction());
		double cosTheta = std::fmin(Dot(-unitDirection, rec.normal), 1.0);
		double sinTheta = std::sqrt(1 - cosTheta * cosTheta);

		bool cannotRefract = ri * sinTheta > 1.0;
		vec3 direction;

		if (cannotRefract || Reflectance(cosTheta, ri) > RandomDouble())
			direction = Reflect(unitDirection, rec.normal);
		else
			direction = Refract(unitDirection, rec.normal, ri);

		scattered = ray(rec.p, direction);
		return true;
	}

private:
	double refractionIndex;

	static double Reflectance(double cosine, double refractionIndex)
	{
		//Use Schlick's approximation for reflectance
		auto r0 = (1 - refractionIndex) / (1 + refractionIndex);
		r0 = r0 * r0;
		return r0 + (1 - r0) * std::pow((1 - cosine), 5);
	}
};