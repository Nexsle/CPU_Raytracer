#pragma once
#include "rtweekend.h"

class vec3
{
public: 
	double e[3];

	vec3() : e{ 0, 0, 0 } {}
	vec3(double e0, double e1, double e2) : e{ e0, e1, e2 } {}

	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }

	vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	double operator[](int i) const { return e[i]; }
	double& operator[](int i) { return e[i]; }

	vec3 operator+=(const vec3& v)
	{
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}

	vec3 operator*=(double t)
	{
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}

	vec3 operator/=(double t)
	{
		return *this *= 1 / t;
	}

	double Length() const
	{
		return std::sqrt(LengthSquared());
	}

	double LengthSquared() const
	{
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	static vec3 random()
	{
		return vec3(RandomDouble(), RandomDouble(), RandomDouble());
	}

	static vec3 random(double min, double max)
	{
		return vec3(RandomDouble(min ,max), RandomDouble(min, max), RandomDouble(min, max));
	}
};

//setting an alias for vec3, usefull for geometric clarity
using point3 = vec3;

//vector utilities function

inline std::ostream& operator<<(std::ostream& out, const vec3& v)
{
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3& u, const vec3& v)
{
	return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v)
{
	return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}


inline vec3 operator*(const vec3& u, const vec3& v)
{
	return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3& v)
{
	return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3& v, double t)
{
	return t * v;
}

inline vec3 operator/(const vec3& v, double t)
{
	return (1 / t) * v ;
}

inline double Dot(const vec3& u, const vec3& v)
{
	return u.e[0] * v.e[0] 
		+ u.e[1] * v.e[1] 
		+ u.e[2] * v.e[2];
}

inline vec3 Cross(const vec3& u, const vec3& v)
{
	return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
				u.e[2] * v.e[0] - u.e[0] * v.e[2],
				u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 UnitVector(const vec3& v)
{
	return v / v.Length();
}

inline vec3 RandomUnitVector()
{
	while (true)
	{
		auto p = vec3::random(-1, 1);
		auto lensq = p.LengthSquared();
		if (1e-160 < lensq && lensq <= 1)
			return p / sqrt(lensq);
	}
}

inline vec3 RandomOnHemisphere(const vec3& normal)
{
	vec3 onUnitSphere = RandomUnitVector();
	if (Dot(onUnitSphere, normal) > 0.0) // same side as normals
		return onUnitSphere;
	else
		return -onUnitSphere;
}