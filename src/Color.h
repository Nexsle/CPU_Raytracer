#pragma once
#include "Vec3.h"
#include "Interval.h"

using color = vec3;

inline double LinearToGamma(double linearComponent)
{
	if (linearComponent > 0)
		return std::sqrt(linearComponent);

	return 0;
}

void WriteColor(std::ostream& out, const color& pixelColor)
{
	auto r = pixelColor.x();
	auto g = pixelColor.y();
	auto b = pixelColor.z();

	//apply a linear to a gamma transform 
	r = LinearToGamma(r);
	g = LinearToGamma(g);
	b = LinearToGamma(b);


	static const interval intensity(0.000, 0.999);
	int rbyte = int(256 * intensity.Clamp(r));
	int gbyte = int(256 * intensity.Clamp(g));
	int bbyte = int(256 * intensity.Clamp(b));

	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}


