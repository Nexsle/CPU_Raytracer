#pragma once
#include "rtweekend.h"
class perlin
{
public:
	perlin()
	{
		for (int i = 0; i < pointCount; i++)
		{
			randvec[i] = UnitVector(vec3::random(-1, 1));
		}

		PerlinGeneratePerm(perm_X);
		PerlinGeneratePerm(perm_Y);
		PerlinGeneratePerm(perm_Z);
	}

	double Noise(const point3& p) const
	{
		auto u = p.x() - std::floor(p.x());
		auto v = p.y() - std::floor(p.y());
		auto w = p.z() - std::floor(p.z());


		auto i = int(std::floor(p.x()));
		auto j = int(std::floor(p.y()));
		auto k = int(std::floor(p.z()));
		vec3 c[2][2][2];

		for (int di = 0; di < 2; di++)
			for (int dj = 0; dj < 2; dj++)
				for (int dk = 0; dk < 2; dk++)
					c[di][dj][dk] = randvec[
						perm_X[(i + di) & 255] ^
							perm_Y[(j + dj) & 255] ^
							perm_Z[(k + dk) & 255]
					];

		return PerlinInterp(c, u, v, w);
	}

	double Turb(const point3& p, int depth) const
	{
		auto accum = 0.0;
		auto temp = p;
		auto weight = 1.0;

		for (int i = 0; i < depth; i++)
		{
			accum += weight * Noise(temp);
			weight *= 0.5;
			temp *= 2;
		}
		return std::fabs(accum);
	}
private:
	static const int pointCount = 256;
	vec3 randvec[pointCount];
	int perm_X[pointCount];
	int perm_Y[pointCount];
	int perm_Z[pointCount];

	static void PerlinGeneratePerm(int* p)
	{
		for (int i = 0; i < pointCount; i++)
			p[i] = i;

		Permute(p, pointCount);
	}

	static void Permute(int* p, int n)
	{
		for (int i = n - 1; i > 0; i--)
		{
			int target = RandomInt(0, i);
			int temp = p[i];
			p[i] = p[target];
			p[target] = temp;
		}
	}

	static double PerlinInterp(const vec3 c[2][2][2], double u, double v, double w)
	{
		//smoothstep formula
		auto uu = u * u * (3 - 2 * u);
		auto vv = v * v * (3 - 2 * v);
		auto ww = w * w * (3 - 2 * w);

		auto accum = 0.0;
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				for (int k = 0; k < 2; k++) 
				{
					vec3 weightV(u - i, v - j, w - k);

					accum += (i * uu + (1 - i) * (1 - uu))
						* (j * vv + (1 - j) * (1 - vv))
						* (k * ww + (1 - k) * (1 - ww))
						* Dot(c[i][j][k], weightV);
				}
					

		return accum;
	}


};