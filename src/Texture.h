#pragma once
#include "rtweekend.h"
#include "rtw_stb_image.h"
#include "Perlin.h"
class texture
{
public:
	virtual ~texture() = default;

	virtual color Value(double u, double v, const point3& p) const = 0;
};


class solidColor : public texture
{
public:
	solidColor(const color& albedo) : albedo(albedo) {}

	solidColor(double red, double green, double blue) : solidColor(color(red, green, blue)) {}

	color Value(double u, double v, const point3& p) const
	{
		return albedo;
	}
private:
	color albedo;
};

class checkerTexture : public texture
{
public:
	checkerTexture(double scale, shared_ptr<texture> even, shared_ptr<texture> odd)
		: invScale(1.0 / scale), even(even), odd(odd) {}

	checkerTexture(double scale, const color& c1, const color& c2)
		: checkerTexture(scale, make_shared<solidColor>(c1), make_shared<solidColor>(c2)) {}

	color Value(double u, double v, const point3& p) const
	{
		auto xInteger = int(std::floor(invScale * p.x()));
		auto yInteger = int(std::floor(invScale * p.y()));
		auto zInteger = int(std::floor(invScale * p.z()));

		bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

		return isEven ? even->Value(u, v, p) : odd->Value(u, v, p);
	}

private:
	double invScale;
	shared_ptr<texture> even;
	shared_ptr<texture> odd;
};


class imageTexture : public texture
{
public:
	imageTexture(const char* filename) : image(filename) {}

	color Value(double u, double v, const point3& p) const override
	{
		//cyan
		if (image.Height() <= 0) return color(0, 1, 1);

		u = interval(0, 1).Clamp(u);
		v = 1.0 - interval(0, 1).Clamp(v);

		auto i = int(u * image.Width());
		auto j = int(v * image.Height());
		auto pixel = image.PixelData(i, j);

		auto colorScale = 1.0 / 255.0;
		return color(colorScale * pixel[0], colorScale * pixel[1], colorScale * pixel[2]);
	}
private:
	rtwImage image;
};

class noiseTexture : public texture
{
public: 
	noiseTexture(double scale) : scale(scale) {}
	
	color Value(double u, double v, const point3& p) const override
	{
		/*return color(1, 1, 1) * noise.Turb(p, 7);*/
		return color(.5, .5, .5) * (1 + std::sin(scale * p.z() + 10 * noise.Turb(p, 7)));
	}

private:
	perlin noise;
	double scale;
};