#pragma once
#include "Hittable.h"

class camera
{
public:
	double aspectRatio = 1.0;
	int imageWidth = 100;
	int samplePerPixel = 10; //num of random sample per pixel
	int maxDepth = 10; //max num of ray bounces

	void Render(const hittable& world)
	{
		Initialize();

		std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

		for (int j = 0; j < imageHeight; j++)
		{
			std::clog << "\rScanline remaining: " << (imageHeight - j) << ' ' << std::flush;
			for (int i = 0; i < imageWidth; i++)
			{
				color pixelColor(0, 0, 0);
				for (int sample = 0; sample < samplePerPixel; sample++)
				{
					ray r = GetRay(i, j);
					pixelColor += RayColor(r,maxDepth, world);
				}

				WriteColor(std::cout, pixelColor * pixelSampleScale);
			}
		}

		std::clog << "\rDone.               \n";
	}

private:
	int imageHeight;
	point3 center; //cam center
	point3 pixel00Loc;
	vec3 pixelDeltaU;
	vec3 pixelDeltaV;
	double pixelSampleScale;


	void Initialize()
	{
		imageHeight = int(imageWidth / aspectRatio);
		imageHeight = imageHeight < 1 ? 1 : imageHeight;

		pixelSampleScale = 1.0 / samplePerPixel;  

		center = point3(0, 0, 0);

		//Viewport
		auto focalLength = 1.0; //distance between camera and viewport
		auto viewportHeight = 2.0;
		auto viewportWidth = viewportHeight * (double(imageWidth) / imageHeight);

		auto viewportU = vec3(viewportWidth, 0, 0);
		auto viewportV = vec3(0, -viewportHeight, 0);
		pixelDeltaU = viewportU / imageWidth;
		pixelDeltaV = viewportV / imageHeight;

		//location of the upper left pixel
		auto viewportUpperLeft = center - vec3(0, 0, focalLength) - viewportU / 2 - viewportV / 2;
		pixel00Loc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);
	}

	color RayColor(const ray& r,int depth, const hittable& world)const
	{
		if (depth <= 0)
			return color(0, 0, 0);

		hitRecord rec;
		if (world.hit(r, interval(0.001, +infinity), rec))
		{
			vec3 direction = rec.normal + RandomUnitVector();
			return 0.5 * RayColor(ray(rec.p, direction), depth-1, world);
		}


		//Sky gradient if no hit
		vec3 unitDirection = UnitVector(r.Direction());
		auto a = 0.5 * (unitDirection.y() + 1.0f);
		return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
	}

	ray GetRay(int i, int j) const
	{
		//randomly sample point around the pixel location i,j

		auto offset = SampleSquare();
		auto pixelSample = pixel00Loc + ((i + offset.x()) * pixelDeltaU) + ((j + offset.y()) * pixelDeltaV);

		auto rayOrigin = center;
		auto rayDirection = pixelSample - rayOrigin;

		return ray(rayOrigin, rayDirection);
	}

	vec3 SampleSquare() const
	{
		return vec3(RandomDouble() - 0.5, RandomDouble() - 0.5, 0);
	}
};